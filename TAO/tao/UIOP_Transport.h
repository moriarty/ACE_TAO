// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     UIOP_Transport.h
//
// = DESCRIPTION
//     UIOP Transport specific processing
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//     Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_TRANSPORT_H
#define TAO_UIOP_TRANSPORT_H

#include "tao/Pluggable.h"

// BALA Temporrary inclusion
//#include "tao/Pluggable_Messaging.h"
#include "tao/GIOP_Utils.h"
//#include "tao/target_identifier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



# if TAO_HAS_UIOP == 1

// Forward decls.
class TAO_UIOP_Handler_Base;
class TAO_UIOP_Client_Connection_Handler;
class TAO_UIOP_Server_Connection_Handler;
class TAO_ORB_Core;

class TAO_Export TAO_UIOP_Transport : public TAO_Transport
{
  // = TITLE
  //   This class acts as a bridge class to the transport specific
  //   connection handler (handler_).
  //
  // = DESCRIPTION
  //   Specialization of the base TAO_Transport class to handle the UIOP
  //   protocol.  This class in turn will be further specialized for
  //   the client and server side.
public:
  TAO_UIOP_Transport (TAO_UIOP_Handler_Base *handler,
                      TAO_ORB_Core *orb_core);
  // Base object's creator method.

  ~TAO_UIOP_Transport (void);
  // Default destructor.

  TAO_UIOP_Handler_Base *&handler (void);
  // Return a reference to the corresponding connection handler.

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual void close_connection (void);
  virtual int idle (void);
  virtual ACE_HANDLE handle (void);
  virtual ACE_Event_Handler *event_handler (void);
  virtual ssize_t send (TAO_Stub *stub,
                        int two_way,
                        const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0);
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0);
  virtual ssize_t send (const u_char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0);
  virtual ssize_t recv (char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0);
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core ,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_wait_time);

  
  CORBA::Boolean 
  send_request_header (const IOP::ServiceContextList &svc_ctx,  
                       CORBA::ULong request_id,
                       CORBA::Octet response_flags,
                       TAO_Target_Specification &spec,
                       const char* opname,
                       TAO_OutputCDR &msg);
  

protected:
  TAO_UIOP_Handler_Base *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.

  TAO_Pluggable_Message_Factory *giop_factory_;
  // The message_factor instance specific for this particular
  // transport protocol.  
};

class TAO_Export TAO_UIOP_Client_Transport : public TAO_UIOP_Transport
{
  // = TITLE
  //   The Transport class used for Client side communication with a
  //   server.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_UIOP_Transport class for client
  //   side.  Methods related to sending one and two way requests
  //   lives here.
public:
  TAO_UIOP_Client_Transport (TAO_UIOP_Client_Connection_Handler *handler,
                             TAO_ORB_Core *orb_core);
  // Constructor.  Note, TAO_UIOP_Handler_Base is the base class for
  // both TAO_UIOP_Client_Connection_Handler and
  // TAO_UIOP_Server_Connection_Handler.

  ~TAO_UIOP_Client_Transport (void);
  // destructor

  TAO_UIOP_Client_Connection_Handler *client_handler (void);
  // return a pointer to the client's connection handler.

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual void start_request (TAO_ORB_Core *orb_core,
                              TAO_Target_Specification &spec,
                              TAO_OutputCDR &output,
                              CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void start_locate (TAO_ORB_Core *orb_core,
                             TAO_Target_Specification &spec,
                             CORBA::ULong request_id,
                             TAO_OutputCDR &output,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_wait_time);
  virtual int handle_client_input (int block = 0,
                                   ACE_Time_Value *max_time_value = 0);
  virtual int register_handler (void);
  // Register the handler with the reactor. This will be called by the
  // Wait Strategy if Reactor is used  for that strategy.

  CORBA::Boolean 
  send_request_header (const IOP::ServiceContextList &svc_ctx,  
                       CORBA::ULong request_id,
                       CORBA::Octet response_flags,
                       TAO_Target_Specification &spec,
                       const char* opname,
                       TAO_OutputCDR &msg);
  
  void messaging_init (TAO_Pluggable_Messaging_Interface *mesg);
  // Initialising the messaging object

private:
  TAO_UIOP_Client_Connection_Handler *client_handler_;
  // pointer to the corresponding client side connection handler.

  TAO_Pluggable_Messaging_Interface *client_mesg_factory_;
  // The message_factor instance specific for this particular
  // transport protocol.
};

// ****************************************************************

class TAO_Export TAO_UIOP_Server_Transport : public TAO_UIOP_Transport
{
  // = TITLE
  //   The Transport class used for server communication with a
  //   connected client.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_UIOP_Transport class for the server side.
  //   methods for reading messages (requests) and sending replies live
  //   here.
public:

  TAO_UIOP_Server_Transport (TAO_UIOP_Server_Connection_Handler *handler,
                             TAO_ORB_Core* orb_core);
  //  Default creator method.

  ~TAO_UIOP_Server_Transport (void);
  // Default destructor

  TAO_UIOP_Server_Connection_Handler *server_handler_;
  // Pointer to the corresponding connection handler.

  TAO_GIOP_Message_State message_state_;
  // This keep the state of the current message, to enable
  // non-blocking reads, fragment reassembly, etc.

  void messaging_init (TAO_Pluggable_Messaging_Interface *mesg);
  // Initialising the messaging object  

 private:
  
  TAO_Pluggable_Messaging_Interface *server_mesg_factory_;
  // This would be server side messaging protocol  
};

# endif  /* TAO_HAS_UIOP == 1 */

#endif  /* TAO_UIOP_TRANSPORT_H */
