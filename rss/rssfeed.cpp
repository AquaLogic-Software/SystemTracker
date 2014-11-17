/****************************************************************************
 **
 ** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 /*
 RSSFeed.cpp

 Provides a widget for displaying news items from RDF news sources.
 RDF is an XML-based format for storing items of information (see
 http://www.w3.org/RDF/ for details).

 The widget itself provides a simple user interface for specifying
 the URL of a news source, and controlling the downloading of news.

 The widget downloads and parses the XML asynchronously, feeding the
 data to an XML reader in pieces. This allows the user to interrupt
 its operation, and also allows very large data sources to be read.
 */

 #include <QtCore>
 #include <QtGui>
 #include <QtNetwork>

 #include "rssfeed.h"

 /*
     Constructs an RSSFeed widget with a simple user interface, and sets
     up the XML reader to use a custom handler class.

     The user interface consists of a line edit, a push button, and a
     list view widget. The line edit is used for entering the URLs of news
     sources; the push button starts the process of reading the
     news.
 */

 RSSFeed::RSSFeed(QWidget *parent)
     : QWidget(parent), currentReply(0)
 {
 #ifdef Q_OS_SYMBIAN
     // Set Internet Access Point
     QNetworkConfigurationManager manager;
     const bool canStartIAP = manager.capabilities() & QNetworkConfigurationManager::CanStartAndStopInterfaces;

     // Is there default access point, use it
     QNetworkConfiguration cfg = manager.defaultConfiguration();
     if (!cfg.isValid() || !canStartIAP) {
         // Available Access Points not found
         QMessageBox::warning(this, "Error", "No access point");
         return;
     }

     m_session = new QNetworkSession(cfg);
     m_session->open();
     m_session->waitForOpened();
 #endif

 #if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_MAEMO_5)
     resize(640,480);
 #endif
 }

 /*
     Starts the network request and connects the needed signals
 */
 void RSSFeed::get(const QUrl &url)
 {
     QNetworkRequest request(url);
     if (currentReply) {
         currentReply->disconnect(this);
         currentReply->deleteLater();
     }
     currentReply = manager.get(request);
     connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
     connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
     connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
 }

 /*
     Starts fetching data from a news source specified in the line
     edit widget.

     The line edit is made read only to prevent the user from modifying its
     contents during the fetch; this is only for cosmetic purposes.
     The fetch button is disabled, the list view is cleared, and we
     define the last list view item to be 0, meaning that there are no
     existing items in the list.

     A URL is created with the raw contents of the line edit and
     a get is initiated.
 */

 void RSSFeed::fetch()
 {

     xml.clear();

     QUrl url("");
     get(url);
 }

 void RSSFeed::metaDataChanged()
 {
     QUrl redirectionTarget = currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
     if (redirectionTarget.isValid()) {
         get(redirectionTarget);
     }
 }

 /*
     Reads data received from the RDF source.

     We read all the available data, and pass it to the XML
     stream reader. Then we call the XML parsing function.
 */

 void RSSFeed::readyRead()
 {
     int statusCode = currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
     if (statusCode >= 200 && statusCode < 300) {
         QByteArray data = currentReply->readAll();
         xml.addData(data);
         parseXml();
     }
 }

 /*
     Finishes processing an HTTP request.

     The default behavior is to keep the text edit read only.

     If an error has occurred, the user interface is made available
     to the user for further input, allowing a new fetch to be
     started.

     If the HTTP get request has finished, we make the
     user interface available to the user for further input.
 */

 void RSSFeed::finished(QNetworkReply *reply)
 {
     Q_UNUSED(reply);
 }

 /*
     Parses the XML data and creates treeWidget items accordingly.
 */
 void RSSFeed::parseXml()
 {
     while (!xml.atEnd()) {
         xml.readNext();
         if (xml.isStartElement()) {
             if (xml.name() == "item")
                 linkString = xml.attributes().value("rss:about").toString();
             currentTag = xml.name().toString();
         } else if (xml.isEndElement()) {
             if (xml.name() == "item") {
                 titleString.clear();
                 linkString.clear();
             }

         } else if (xml.isCharacters() && !xml.isWhitespace()) {
             if (currentTag == "title")
                 titleString += xml.text().toString();
             else if (currentTag == "link")
                 linkString += xml.text().toString();
         }
     }
     if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
         qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
     }
 }

 void RSSFeed::error(QNetworkReply::NetworkError)
 {
     qWarning("error retrieving RSS feed");
     currentReply->disconnect(this);
     currentReply->deleteLater();
     currentReply = 0;
 }
