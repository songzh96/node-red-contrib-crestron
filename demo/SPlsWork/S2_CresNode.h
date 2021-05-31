#ifndef __S2_CRESNODE_H__
#define __S2_CRESNODE_H__




/*
* Constructor and Destructor
*/

/*
* DIGITAL_INPUT
*/

#define __S2_CresNode_DIGITAL_VALUE_DIG_INPUT 0
#define __S2_CresNode_DIGITAL_VALUE_ARRAY_LENGTH 20

/*
* ANALOG_INPUT
*/



#define __S2_CresNode_ANALOG_VALUE_ANALOG_INPUT 0
#define __S2_CresNode_ANALOG_VALUE_ARRAY_LENGTH 20
#define __S2_CresNode_STRING_VALUE_STRING_INPUT 20
#define __S2_CresNode_STRING_VALUE_ARRAY_NUM_ELEMS 20
#define __S2_CresNode_STRING_VALUE_ARRAY_NUM_CHARS 9999
CREATE_STRING_ARRAY( S2_CresNode, __STRING_VALUE, __S2_CresNode_STRING_VALUE_ARRAY_NUM_ELEMS, __S2_CresNode_STRING_VALUE_ARRAY_NUM_CHARS );

/*
* DIGITAL_OUTPUT
*/

#define __S2_CresNode_DIGITAL_TRUE_DIG_OUTPUT 0
#define __S2_CresNode_DIGITAL_TRUE_ARRAY_LENGTH 20
#define __S2_CresNode_DIGITAL_FALSE_DIG_OUTPUT 20
#define __S2_CresNode_DIGITAL_FALSE_ARRAY_LENGTH 20

/*
* ANALOG_OUTPUT
*/


#define __S2_CresNode_ANALOG_SET_ANALOG_OUTPUT 0
#define __S2_CresNode_ANALOG_SET_ARRAY_LENGTH 20
#define __S2_CresNode_STRING_SET_STRING_OUTPUT 20
#define __S2_CresNode_STRING_SET_ARRAY_LENGTH 20

/*
* Direct Socket Variables
*/

#define __S2_CresNode_MYSERVER_SOCKET 61
#define __S2_CresNode_MYSERVER_STRING_MAX_LEN 9999
START_SOCKET_DEFINITION( S2_CresNode, __MYSERVER )
{
   int SocketStatus;
   enum ESplusSocketType eSocketType;
   int SocketID;
   void *SocketPtr;
CREATE_SOCKET_STRING( S2_CresNode, SocketRxBuf, __S2_CresNode_MYSERVER_STRING_MAX_LEN );
};



/*
* INTEGER_PARAMETER
*/
#define __S2_CresNode_PORT_INTEGER_PARAMETER 11
/*
* SIGNED_INTEGER_PARAMETER
*/
/*
* LONG_INTEGER_PARAMETER
*/
/*
* SIGNED_LONG_INTEGER_PARAMETER
*/
/*
* INTEGER_PARAMETER
*/
/*
* SIGNED_INTEGER_PARAMETER
*/
/*
* LONG_INTEGER_PARAMETER
*/
/*
* SIGNED_LONG_INTEGER_PARAMETER
*/
/*
* STRING_PARAMETER
*/
#define __S2_CresNode_IPADDR$_STRING_PARAMETER 10
#define __S2_CresNode_IPADDR$_PARAM_MAX_LEN 16
CREATE_STRING_STRUCT( S2_CresNode, __IPADDR$, __S2_CresNode_IPADDR$_PARAM_MAX_LEN );


/*
* INTEGER
*/


/*
* LONG_INTEGER
*/


/*
* SIGNED_INTEGER
*/


/*
* SIGNED_LONG_INTEGER
*/


/*
* STRING
*/
#define __S2_CresNode_INTERNALRXBUFFER_STRING_MAX_LEN 65000
CREATE_STRING_STRUCT( S2_CresNode, __INTERNALRXBUFFER, __S2_CresNode_INTERNALRXBUFFER_STRING_MAX_LEN );

/*
* STRUCTURE
*/

START_GLOBAL_VAR_STRUCT( S2_CresNode )
{
   void* InstancePtr;
   struct GenericOutputString_s sGenericOutStr;
   unsigned short LastModifiedArrayIndex;

   DECLARE_IO_ARRAY( __DIGITAL_VALUE );
   DECLARE_IO_ARRAY( __DIGITAL_TRUE );
   DECLARE_IO_ARRAY( __DIGITAL_FALSE );
   DECLARE_IO_ARRAY( __ANALOG_VALUE );
   DECLARE_IO_ARRAY( __ANALOG_SET );
   DECLARE_IO_ARRAY( __STRING_SET );
   short __DOSERVERCONNECTED;
   DECLARE_STRING_STRUCT( S2_CresNode, __INTERNALRXBUFFER );
   DECLARE_STRING_ARRAY( S2_CresNode, __STRING_VALUE );
   DECLARE_SOCKET( S2_CresNode, __MYSERVER );
   DECLARE_STRING_STRUCT( S2_CresNode, __IPADDR$ );
};

START_NVRAM_VAR_STRUCT( S2_CresNode )
{
};



#endif //__S2_CRESNODE_H__
