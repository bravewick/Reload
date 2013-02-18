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
 * @file BroadcastHandler.cpp
 *
 * Functionality for Automatic Server Discovery
 *
 * Author: Kostas Tsolakis
 */
#include "BroadcastHandler.h"

#include <maheap.h>
#include "Convert.h"
#include "Log.h"

using namespace MAUtil;

BroadcastHandler::BroadcastHandler(LoginScreen *loginScreen) :
	mDatagramSocket(this)
{
	mLoginScreen = loginScreen;

	// Initiallize broadcastAddress
	mBroadcastAddress.family = CONN_FAMILY_INET4;
	mBroadcastAddress.inet4.addr = (255 << 24) | (255 << 16) | (255 << 8) | 255;
	mBroadcastAddress.inet4.port = 41234;

	this->mServerAddress = "";
	sprintf(mBroadcastedData,"%s","CONREQ");
}

BroadcastHandler::~BroadcastHandler()
{ }

void BroadcastHandler::findServer()
{
	int res = mDatagramSocket.connect("datagram://");
	lprintfln("@@@@@@@ connect: %i\n", res);
	if(res > 0)
	{
		broadcast();
	}
}

void BroadcastHandler::closeConnection()
{
	mDatagramSocket.close();
}

void BroadcastHandler::broadcast() {

	mDatagramSocket.writeTo(mBroadcastedData, 6, mBroadcastAddress);
}
/**
 * The connect() operation has completed.
 */
void BroadcastHandler::connectFinished(Connection* conn, int result)
{
	lprintfln("@@@ connection:%d",result);
	if(result < 0) {
		return;
	}
	else {

		MAConnAddr address;
		address.family = CONN_FAMILY_INET4;
		address.inet4.addr = (255 << 24) | (255 << 16) | (255 << 8) | 255;
		address.inet4.port = 41234;

		mDatagramSocket.writeTo(mBroadcastedData, 6, address);
	}
}

void BroadcastHandler::connRecvFinished(Connection* conn, int result)
{

	lprintfln("connRecvFinished: %i\n", result);
	if(result < 0)
	{
		return;
	}
	else
	{
		for(int i = 0; i < result; i++)
		{
			mServerAddress[i] = mBuffer[i];
		}
		mServerAddress[result] = '\0';
		lprintfln("SERVER RESPONSE:%s",mBuffer);
		mLoginScreen->addServerListItem(mServerAddress);
	}
}

void BroadcastHandler::connWriteFinished(Connection* conn, int result)
{
	mDatagramSocket.recvFrom(mBuffer, sizeof(mBuffer), &mAddress);
}
