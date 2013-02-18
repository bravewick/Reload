/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file BroadcastHandler.h
 *
 * Functionality for Automatic Server Discovery
 *
 * Author: Kostas Tsolakis
 */

#ifndef BROADCAST_HANDLER_H
#define BROADCAST_HANDLER_H

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Connection.h>
#include "LoginScreen.h"

class LoginScreen;

/**
 * Class that broadcasts message for server discovery
 */
class BroadcastHandler :
	public MAUtil::ConnectionListener
{
public:
	/**
	 * Constructor.
	 */
	BroadcastHandler(LoginScreen *mLoginScreen);

	/**
	 * Destructor.
	 */
	virtual ~BroadcastHandler();

	/**
	 * Method that starts a broadcast for server discovery
	 */
	void BroadcastHandler::findServer();

	/**
	 * The socket->connect() operation has finished.
	 */
	void connectFinished(MAUtil::Connection* conn, int result);

	/**
	 * We received a TCP message from the server.
	 * Not used.
	 */
	void connRecvFinished(MAUtil::Connection* conn, int result);

	/**
	 * Socket write operation has finished.
	 */
	void connWriteFinished(MAUtil::Connection* conn, int result);

	void closeConnection();

private:
	/**
	 * Method that executes writeTo using UDP socket connection
	 */
	void BroadcastHandler::broadcast();

	/**
	 * The UDP socket used for registering with the server
	 * and listening for commands from the server.
	 */
	MAUtil::Connection mDatagramSocket;

	/**
	 * The data that are broadcasted to the network
	 */
	char mBroadcastedData[6];

	char mBuffer[512];

	/**
	 * The sender's address
	 */
	MAConnAddr mAddress;

	MAConnAddr mBroadcastAddress;

	MAUtil::String mServerAddress;

	LoginScreen *mLoginScreen;
};

#endif
