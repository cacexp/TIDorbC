//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServerListener.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"

#include "TIDSocket.h"

#undef ERROR




TIDorb::core::comm::ServerListener::ServerListener(ConnectionManager* the_manager,
                                                   TIDThr::ThreadGroup* group)
  throw (TIDThr::SystemException, CORBA::SystemException)
  : TIDThr::Thread(group, NULL, ""), conf(the_manager->orb()->conf())
{
  do_shutdown   = false;
  server_socket = NULL;
  manager       = the_manager;
  _orb          = manager->orb();


  TIDSocket::InetAddress* inet = NULL;
  char* localhost = NULL;
  char* interface = NULL;
  char* ip_iface  = NULL;
  
  try {
    if (conf.ip_address == NULL){
      // Bug #343 Allow to reuse port
      server_socket = new TIDSocket::ServerSocket(conf.prefer_ipv6);
      server_socket->setReuseAddress(true);
      try{
      	TIDSocket::InetSocketAddress inet(conf.orb_port, conf.prefer_ipv6);
        server_socket->bind((TIDSocket::SocketAddress*) &inet, 
                            conf.server_socket_backlog, interface);
      }
      catch(TIDSocket::IllegalArgumentException& e) {
          throw TIDSocket::IOException(e.what());
      }
    }
    else {
      if (conf.prefer_ipv6) {
      	string aux = conf.ip_address;
      	if (strcmp(aux.c_str(),"::1") != 0) {
          aux += "%";
          aux += conf.iface;
        }
        ip_iface = CORBA::string_dup(aux.c_str());
        inet = TIDSocket::InetAddress::getByName(ip_iface);
        interface = CORBA::string_dup(conf.iface);
      }
      else
        inet = TIDSocket::InetAddress::getByName(conf.ip_address);
      // Bug #343 Allow to reuse port
      server_socket = new TIDSocket::ServerSocket(conf.prefer_ipv6);
      server_socket->setReuseAddress(true);
      try {
        TIDSocket::InetSocketAddress addr(conf.orb_port, conf.prefer_ipv6);
        if (inet) 
          addr = TIDSocket::InetSocketAddress(inet, conf.orb_port);
        server_socket->bind((TIDSocket::SocketAddress*) &addr, 
                            conf.server_socket_backlog, interface);
      } catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
      delete inet;
      inet = NULL;
    }

    server_socket->setReceiveBufferSize(conf.tcp_buffer_size);

    if(conf.host != NULL)
      listen_points.insert(TIDorb::core::comm::iiop::ListenPoint(
                                              conf.host, server_socket->getLocalPort()));
    else{
      if (conf.prefer_ipv6) {
        if (conf.ip_address == NULL)
          inet = TIDSocket::InetAddress::getByName("::1"); //ip6-localhost");
        else
          inet = TIDSocket::InetAddress::getByName(ip_iface);

        if (ip_iface != NULL)
          localhost = CORBA::string_dup(ip_iface);
        else
          localhost = inet->getHostAddress();
          
        listen_points.insert(TIDorb::core::comm::iiop::ListenPoint(
                                               localhost, server_socket->getLocalPort()));
        delete inet;
        delete[] localhost;
      }

      inet = TIDSocket::InetAddress::getLocalHost();
      localhost = inet->getHostAddress();
      // TODO: if there is more than 1 interface by address...
//       TIDSocket::InetAddressList* inet_list = 
//         TIDSocket::InetAddress::getAllByName(inet->getCanonicalHostName());
//       TIDSocket::InetAddressList::iterator it  = inet_list->begin();
//       TIDSocket::InetAddressList::iterator end = inet_list->end();
//       while (it != end) {     
      listen_points.insert(TIDorb::core::comm::iiop::ListenPoint(
                                               localhost, server_socket->getLocalPort()));
//       listen_points.insert(TIDorb::core::comm::iiop::ListenPoint(//(*it).getHostAddress(), 
//                                                                  inet->getCanonicalHostName(),
//                                                                  server_socket->getLocalPort()));

//         it++;
//       }
      delete[] localhost;
      CORBA::string_free(interface);
      CORBA::string_free(ip_iface);
      delete inet;
      inet = NULL;
    }

  } catch (const TIDSocket::IOException& e) {

    delete[] localhost;
    delete server_socket;
    delete inet;
    throw CORBA::INITIALIZE("Cannot open ServerSocket");
  }
}




TIDorb::core::comm::ServerListener::~ServerListener()
  throw (TIDThr::SystemException)
{
  if(server_socket)
    delete server_socket;
}




const TIDorb::core::comm::iiop::ListenPointSet&
  TIDorb::core::comm::ServerListener::get_listen_points()
{
  return listen_points;
}




void TIDorb::core::comm::ServerListener::shutdown()
{
  try {
    TIDThr::Synchronized synchro(*this);
    if(!do_shutdown) {
      do_shutdown = true;
      server_socket->close();
      delete server_socket;
      server_socket = NULL;
    }
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "ServerListener at ";
      TIDorb::core::comm::iiop::ListenPointSet::iterator it  = listen_points.begin();
      TIDorb::core::comm::iiop::ListenPointSet::iterator end = listen_points.end();
      while  (it != end) {
        msg << (*it).toString() << " ";
        it++;
      }
      msg << " shutdown!";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  } catch(...) {
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "ServerListener at ";
      TIDorb::core::comm::iiop::ListenPointSet::iterator it  = listen_points.begin();
      TIDorb::core::comm::iiop::ListenPointSet::iterator end = listen_points.end();
      while  (it != end) {
        msg << (*it).toString() << " ";
        it++;
      }
      msg << " shutdown!";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(),CORBA::UNKNOWN());
    }
  }

}




void TIDorb::core::comm::ServerListener::reset_server_socket()
{
  try {
    if(server_socket != NULL) {
      server_socket->close();
      delete server_socket;
      server_socket = NULL;
    }
  } catch (...) {
  }

  TIDSocket::InetAddress* inet = NULL;
  const char* interface = NULL;
  try {
    if(conf.ip_address == NULL) {
      server_socket = // Bug #343 Allow to reuse port
        // new TIDSocket::ServerSocket(listen_point._port, _orb->conf().server_socket_backlog);
        new TIDSocket::ServerSocket(conf.prefer_ipv6);
      server_socket->setReuseAddress(true);
      try{
        //TIDSocket::InetSocketAddress inet(listen_point._port);
        // All listen points have the same port, so we can take the first.
        TIDorb::core::comm::iiop::ListenPointSet::iterator it  = listen_points.begin();
        TIDSocket::InetSocketAddress inet((*it)._port, conf.prefer_ipv6);
        server_socket->bind((TIDSocket::SocketAddress*) &inet, 
                            conf.server_socket_backlog, interface);
      }
      catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
    else {
      inet = TIDSocket::InetAddress::getByName(conf.ip_address);
      server_socket = // Bug #343 Allow to reuse port
        // new TIDSocket::ServerSocket(listen_point._port, _orb->conf().server_socket_backlog, inet);
        new TIDSocket::ServerSocket(conf.prefer_ipv6);
      server_socket->setReuseAddress(true);
      try {
        TIDSocket::InetSocketAddress addr(conf.orb_port, conf.prefer_ipv6);
        if (inet) 
          addr = TIDSocket::InetSocketAddress(inet, conf.orb_port);
        if (conf.prefer_ipv6) {
          string point = conf.ip_address;
          string::size_type colon_position = point.find('%');
          if (colon_position != string::npos)
            interface = point.substr(colon_position + 1).c_str();
        }
        server_socket->bind((TIDSocket::SocketAddress*) &addr, 
                            conf.server_socket_backlog, interface);
      } catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
  } catch (...) {
    delete inet;
    throw  CORBA::INITIALIZE("ServerListener::reset_server_socket(): Cannot restore ServerSocket ");
  }

  delete inet;
}




void TIDorb::core::comm::ServerListener::run()
 {
    bool connected = true;
    // bool shutdown = false;

    while (!do_shutdown){

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer buffer;
        buffer << "Accepting connections at ";
        TIDorb::core::comm::iiop::ListenPointSet::iterator it  = listen_points.begin();
        TIDorb::core::comm::iiop::ListenPointSet::iterator end = listen_points.end();
        while  (it != end) {
          buffer << (*it).toString() << " ";
          it++;
        }
        buffer << "..." << ends;
        _orb->print_trace(TIDorb::util::TR_DEBUG, buffer.str().data());
      }

      while (connected) {

        TIDSocket::Socket* client_socket = NULL;

        try {

          if (server_socket != NULL)
            client_socket = server_socket->accept();
          else
            connected = false;

        } catch (const TIDSocket::SocketException& e) {
          connected = false;
          if (_orb->trace != NULL) {
            _orb->print_trace(TIDorb::util::TR_ERROR, "ServerSocket closed");
          }

        } catch (...) {
          connected = false;
          if (_orb->trace != NULL) {
            _orb->print_trace(TIDorb::util::TR_ERROR, "Error in ServerSocket: ",
                              CORBA::UNKNOWN());
          }
        }

        try {
          if(client_socket != NULL)
            manager->createServerConnection(client_socket);

        } catch (...) {

          delete client_socket;

          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_ERROR, "Error creating ServerConnection");
          }
        }
      }

      if(!do_shutdown) {

        // RECONNECT
        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_DEBUG, "Trying to reconnect ServerSocket");
        }
        try {

          reset_server_socket();
          connected = true;

          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_DEBUG, "ServerSocket reconnected ");
          }

        } catch (...)
        {
          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_DEBUG, "ServerSocket reconnect error: ",
                              CORBA::UNKNOWN());
          }

          try {
            Thread::sleep(conf.server_socket_reconnect);
          } catch (...) {}

        }
      }
    }
}



