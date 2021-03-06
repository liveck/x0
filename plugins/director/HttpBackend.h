/* <plugins/director/HttpBackend.h>
 *
 * This file is part of the x0 web server project and is released under GPL-3.
 * http://www.xzero.io/
 *
 * (c) 2009-2012 Christian Parpart <trapni@gentoo.org>
 */

#pragma once

#include "Backend.h"

/*!
 * implements the HTTP backend.
 *
 * \see FastCgiBackend
 */
class HttpBackend : public Backend {
private:
	class ProxyConnection;

public:
	HttpBackend(Director* director, const std::string& name, const x0::SocketSpec& socketSpec, size_t capacity);
	~HttpBackend();

	virtual const std::string& protocol() const;
	virtual bool process(x0::HttpRequest* r);

	const x0::SocketSpec& socketSpec() const { return socketSpec_; }

private:
	ProxyConnection* acquireConnection();
};


