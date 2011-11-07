/*
  This file is part of the PhantomJS project from Ofi Labs.

  Copyright (C) 2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Milian Wolff <milian.wolff@kdab.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>

///TODO: is this ok, or should it be put into .cpp
///      can be done by introducing a WebServerPrivate *d;
#include "mongoose.h"

class Config;

class WebServerRequest;
class WebServerResponse;

class WebServer : public QObject
{
    Q_OBJECT

public:
    WebServer(QObject *parent, const Config *config);
    virtual ~WebServer();

public slots:
    ///WARNING: must not be the same name as in the javascript api...
    void listenOnPort(const QString &port);
    void handleRequest(mg_event event, mg_connection* conn, const mg_request_info* request,
                       bool* handled);

signals:
    void newRequest(QObject *request, QObject *response);

private:
    Config *m_config;
    mg_context *m_ctx;
};

class WebServerRequest : public QObject
{
    Q_OBJECT

public:
    WebServerRequest(const mg_request_info *request);

public slots:
    /// @return request method, i.e. Get/Post
    QString method() const;
    QString httpVersion() const;
    int statusCode() const;
    bool isSSL() const;
    QString url() const;
    QString queryString() const;
    QString remoteIP() const;
    int remotePort() const;
    QString remoteUser() const;

    //TODO: better javascript api that allows easy "for(i in headers)" iteration?
    //see e.g.: http://www.qtcentre.org/threads/31298-QtScript-bindings-to-advanced-containers-%28QMap-QList-etc%29
    int headers() const;
    QString headerName(int header) const;
    QString headerValue(int header) const;

private:
    const mg_request_info *m_request;
};

/**
 * TODO: copy more from http://nodejs.org/docs/latest/api/http.html#http.ServerResponse
 */
class WebServerResponse : public QObject
{
    Q_OBJECT

public:
    WebServerResponse(mg_connection *conn);

public slots:
    ///TODO: improve API
    void writeHeaders(const QString& headers);
    ///TODO: implictly write default headers if not called yet
    void writeBody(const QString& body);

private:
    mg_connection *m_conn;
};

#endif // WEBSERVER_H
