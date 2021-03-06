//////////////////////////////////////////////////////////////////////////////////
//
// File:        PolicyContextManagerImpl.h
// Description: PolicyContextManager implementation.
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

#include "TIDorb/core.h"

#ifndef __TIDorb_core_PolicyContextManagerImpl_H_
#define __TIDorb_core_PolicyContextManagerImpl_H_

#include "CORBA.h"

#include <map>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif


namespace TIDorb {
namespace core {

class PolicyContextManagerImpl: public virtual TIDThr::RecursiveMutex
{
protected:
  TIDorb::core::TIDORB* m_orb;
  
  int CLEANUP_ROUND;

  typedef map<TIDThr::ThreadHandle, TIDorb::core::PolicyContext*> ThreadContextMapT;
  ThreadContextMapT m_thread_contexts;   
  
  int m_cleanup_round;

  TIDorb::core::ConfORB& m_conf;

public:
 
  PolicyContextManagerImpl(TIDorb::core::TIDORB* orb)
    throw (TIDThr::SystemException);

  ~PolicyContextManagerImpl()
    throw (TIDThr::SystemException);

  TIDorb::core::PolicyContext* getThreadContext(TIDThr::Thread* th);

  TIDorb::core::PolicyContext* tryToGetThreadContext(TIDThr::Thread* th);

private:
  void clear_contexts();

};

}
}

#endif

