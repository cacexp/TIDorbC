///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Selector.C
// Description: This file contains the Selector class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"
#include <sys/param.h>
#include <string.h>
#include <errno.h>
#include <algorithm>


#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::max;
#endif




namespace TIDSocket
{




//
// Constructor
//
Selector::Selector()
    throw()
{
    FD_ZERO(&_key_set);
    FD_ZERO(&_sel_key_set);
    _max_fd = -1;
}




//
// keys()
//
const fd_set* Selector::keys()
    throw()
{
    return (const fd_set*) &_key_set;
}




//
// select()
//
int Selector::select()
    throw(IOException)
{
    return select((time_t) 0);
}




//
// select()
//
int Selector::select(time_t timeout)
    throw(IOException, IllegalArgumentException)
{
    if (timeout < 0)
    {
        throw IllegalArgumentException("Negative timeout");
    }

    // Calcula el timeout en microsegundos
    struct timeval  tval_timeout;
    struct timeval* timeoutptr = NULL;
    if (timeout)
    {
        tval_timeout.tv_sec  = ((long) timeout) / 1000L;
        tval_timeout.tv_usec = 1000L * (((long) timeout) % 1000L);
        timeoutptr = &tval_timeout;
    }

    return implSelect(timeoutptr);
}




//
// selectNow()
//
int Selector::selectNow()
    throw(IOException)
{
    timeval timeout = { 0L, 0L };
    return implSelect(&timeout);
}




//
// selectedKeys()
//
const fd_set* Selector::selectedKeys()
    throw()
{
    return (const fd_set*) &_sel_key_set;
}




//
// implSelect()
//
int Selector::implSelect(struct timeval* timeout)
    throw(IOException)
{
    int fd;
    int ops;

    // Prepara rset y wset
    fd_set rset;
    fd_set wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    for (fd = 0; fd <= _max_fd; fd++)
    {
        if (FD_ISSET(fd, &_key_set))
        {
            FD_SET(fd, &rset);
            FD_SET(fd, &wset);
        }
    }

    // select
    int count = ::select(_max_fd + 1, &rset, &wset, NULL, timeout);
    if (count == -1)
    {
        throw IOException("select() error", errno);
    }

    // Actualiza todas la SelectionKeys registradas y calcula _sel_key_set;
    FD_ZERO(&_sel_key_set);
    for (size_t i = 0; i < _reg_keys.size(); i++)
    {
        SelectionKey& key = _reg_keys[i];
        ops = key._interest;
        fd  = key._fd;

        if ((ops | SelectionKey::OP_ACCEPT) && FD_ISSET(fd, &wset))
        {
            key._ready |= SelectionKey::OP_ACCEPT;
            FD_SET(fd, &_sel_key_set);
        }
        if ((ops | SelectionKey::OP_CONNECT) && FD_ISSET(fd, &rset))
        {
            key._ready |= SelectionKey::OP_CONNECT;
            FD_SET(fd, &_sel_key_set);
        }
        if ((ops | SelectionKey::OP_READ) && FD_ISSET(fd, &rset))
        {
            key._ready |= SelectionKey::OP_READ;
            FD_SET(fd, &_sel_key_set);
        }
        if ((ops | SelectionKey::OP_WRITE) && FD_ISSET(fd, &wset))
        {
            key._ready |= SelectionKey::OP_WRITE;
            FD_SET(fd, &_sel_key_set);
        }
    }

    return count;
}




//
// registerKey()
//
SelectionKey& Selector::registerKey(int fd, int ops, SelectableChannel* channel)
    throw()
{
    FD_SET(fd, &_key_set);
    _max_fd = max(_max_fd, fd);
    _reg_keys.push_back(SelectionKey(fd, this, channel, ops));

    return _reg_keys.back();
}



//
// static open()
//
Selector* Selector::open()
    throw(IOException)
{
    return new Selector;
}




} //namespace TIDSocket
