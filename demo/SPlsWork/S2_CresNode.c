#include "TypeDefs.h"
#include "Globals.h"
#include "FnctList.h"
#include "Library.h"
#include "SimplSig.h"
#include "S2_CresNode.h"

FUNCTION_MAIN( S2_CresNode );
EVENT_HANDLER( S2_CresNode );
DEFINE_ENTRYPOINT( S2_CresNode );




static void S2_CresNode__STARTSERVER ( ) 
    { 
    /* Begin local function variable declarations */
    
    short  __STATUS; 
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 57 );
    __STATUS = SocketServerStartListen( INSTANCE_PTR( S2_CresNode ) , SOCKET_PTR ( S2_CresNode ,__MYSERVER) , GLOBAL_STRING_STRUCT( S2_CresNode, __IPADDR$  )  , GetIntegerParameter( INSTANCE_PTR( S2_CresNode ), __S2_CresNode_PORT_INTEGER_PARAMETER )  ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 58 );
    if ( (__STATUS < 0)) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 59 );
        Print( INSTANCE_PTR( S2_CresNode ) , 45, "Error listening to %s on port %u (status: %d)", GLOBAL_STRING_STRUCT( S2_CresNode, __IPADDR$  )  , (unsigned short )( GetIntegerParameter( INSTANCE_PTR( S2_CresNode ), __S2_CresNode_PORT_INTEGER_PARAMETER )  ), (short)( __STATUS )) ; 
        } 
    
    else 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 61 );
        Print( INSTANCE_PTR( S2_CresNode ) , 44, "Server started to %s on port %u (status: %d)", GLOBAL_STRING_STRUCT( S2_CresNode, __IPADDR$  )  , (unsigned short )( GetIntegerParameter( INSTANCE_PTR( S2_CresNode ), __S2_CresNode_PORT_INTEGER_PARAMETER )  ), (short)( __STATUS )) ; 
        } 
    
    
    S2_CresNode_Exit__STARTSERVER:
/* Begin Free local function variables */
    /* End Free local function variables */
    
    }
    
static void S2_CresNode__SOCKETTX ( struct StringHdr_s* __SERVERTX ) 
    { 
    /* Begin local function variable declarations */
    
    short  __ISTATUS; 
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 68 );
    __ISTATUS = SocketSend( INSTANCE_PTR( S2_CresNode ) , SOCKET_PTR( S2_CresNode , __MYSERVER ) , LOCAL_STRING_STRUCT( __SERVERTX  )  ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 70 );
    if ( (__ISTATUS < 0)) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 71 );
        Print( INSTANCE_PTR( S2_CresNode ) , 31, "Error Sending to MyServer: %d\r\n", (short)( __ISTATUS )) ; 
        } 
    
    else 
        { 
        } 
    
    
    S2_CresNode_Exit__SOCKETTX:
/* Begin Free local function variables */
    /* End Free local function variables */
    
    }
    
static void S2_CresNode__CMDBUILDER ( struct StringHdr_s* __SERVICE , struct StringHdr_s* __ID , struct StringHdr_s* __CMD , struct StringHdr_s* __VALUE ) 
    { 
    /* Begin local function variable declarations */
    
    CREATE_STRING_STRUCT( S2_CresNode, __CMDBUILDER$, 1000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __CMDBUILDER$ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__, sizeof( ":" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__, sizeof( "*" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 1000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __CMDBUILDER$ );
    INITIALIZE_LOCAL_STRING_STRUCT( __CMDBUILDER$, 1000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__, ":" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__, "*" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 1000 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 79 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,16 , "%s%s%s%s%s%s%s%s"  , LOCAL_STRING_STRUCT( __SERVICE  )  ,   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   ,  LOCAL_STRING_STRUCT( __ID  )  ,   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   ,  LOCAL_STRING_STRUCT( __CMD  )  ,   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   ,  LOCAL_STRING_STRUCT( __VALUE  )  ,   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ )    )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __CMDBUILDER$  )   ,2 , "%s"  , __FN_DST_STR__ ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 80 );
    S2_CresNode__SOCKETTX (  (struct StringHdr_s* )  LOCAL_STRING_STRUCT( __CMDBUILDER$  )  ) ; 
    
    S2_CresNode_Exit__CMDBUILDER:
/* Begin Free local function variables */
    FREE_LOCAL_STRING_STRUCT( __CMDBUILDER$ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ );
    FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
    /* End Free local function variables */
    
    }
    
static void S2_CresNode__CMDPARSER ( struct StringHdr_s* __SERVERRX ) 
    { 
    /* Begin local function variable declarations */
    
    CREATE_STRING_STRUCT( S2_CresNode, __SERVICE, 1000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SERVICE );
    
    CREATE_STRING_STRUCT( S2_CresNode, __ID, 1000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __ID );
    
    CREATE_STRING_STRUCT( S2_CresNode, __CMD, 100 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __CMD );
    
    CREATE_STRING_STRUCT( S2_CresNode, __VALUE, 1000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __VALUE );
    
    unsigned short  __IDINT; 
    unsigned short  __VALUEINT; 
    unsigned short  __PULSETIME; 
    CREATE_STRING_STRUCT( S2_CresNode, __RETURNVALUE$, 1000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __RETURNVALUE$ );
    
    unsigned short  __RETURNVALUE; 
    unsigned short  __TEMP; 
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__, sizeof( ":" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__, sizeof( "Digital" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_2__, sizeof( "Set" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_2__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_3__, sizeof( "Analog" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_3__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_4__, sizeof( "String" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_4__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_5__, sizeof( "get" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_5__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_6__, sizeof( "Event" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_6__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 65535 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1, 65535 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SERVICE );
    INITIALIZE_LOCAL_STRING_STRUCT( __SERVICE, 1000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __ID );
    INITIALIZE_LOCAL_STRING_STRUCT( __ID, 1000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __CMD );
    INITIALIZE_LOCAL_STRING_STRUCT( __CMD, 100 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __VALUE );
    INITIALIZE_LOCAL_STRING_STRUCT( __VALUE, 1000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __RETURNVALUE$ );
    INITIALIZE_LOCAL_STRING_STRUCT( __RETURNVALUE$, 1000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__, ":" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__, "Digital" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_2__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_2__, "Set" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_3__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_3__, "Analog" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_4__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_4__, "String" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_5__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_5__, "get" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_6__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_6__, "Event" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 65535 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__1, 65535 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 94 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Remove ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,  LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   , LOCAL_STRING_STRUCT( __SERVERRX  )    , 1  )  )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __SERVICE  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 95 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Left ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , LOCAL_STRING_STRUCT( __SERVICE  )  , (Len( LOCAL_STRING_STRUCT( __SERVICE  )  ) - 1))  )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __SERVICE  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 97 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Remove ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,  LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   , LOCAL_STRING_STRUCT( __SERVERRX  )    , 1  )  )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __ID  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 98 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Left ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , LOCAL_STRING_STRUCT( __ID  )  , (Len( LOCAL_STRING_STRUCT( __ID  )  ) - 1))  )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __ID  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 99 );
    __IDINT = Atoi( LOCAL_STRING_STRUCT( __ID  )  ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 101 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Remove ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,  LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   , LOCAL_STRING_STRUCT( __SERVERRX  )    , 1  )  )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __CMD  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 102 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Left ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , LOCAL_STRING_STRUCT( __CMD  )  , (Len( LOCAL_STRING_STRUCT( __CMD  )  ) - 1))  )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __CMD  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 104 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,2 , "%s"  , LOCAL_STRING_STRUCT( __SERVERRX  )   )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __VALUE  )   ,2 , "%s"  , __FN_DST_STR__ ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 105 );
    __VALUEINT = Atoi( LOCAL_STRING_STRUCT( __VALUE  )  ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 113 );
    if ( ((CompareStrings( LOCAL_STRING_STRUCT( __SERVICE  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ ) , 1 ) == 0) && (CompareStrings( LOCAL_STRING_STRUCT( __CMD  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_2__ ) , 1 ) == 0))) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 115 );
        if ( (__VALUEINT == 1)) 
            { 
            UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 116 );
            Pulse ( INSTANCE_PTR( S2_CresNode ) , 20, CheckIOArrayBounds( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__DIGITAL_TRUE , __IDINT) ) ; 
            } 
        
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 120 );
        if ( (__VALUEINT == 0)) 
            { 
            UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 121 );
            Pulse ( INSTANCE_PTR( S2_CresNode ) , 20, CheckIOArrayBounds( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__DIGITAL_FALSE , __IDINT) ) ; 
            } 
        
        } 
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 127 );
    if ( ((CompareStrings( LOCAL_STRING_STRUCT( __SERVICE  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_3__ ) , 1 ) == 0) && (CompareStrings( LOCAL_STRING_STRUCT( __CMD  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_2__ ) , 1 ) == 0))) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 130 );
        if ( ((__VALUEINT <= 65535) && (__VALUEINT >= 0))) 
            { 
            UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 131 );
            SetOutputArrayElement ( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__ANALOG_SET ,__IDINT, __VALUEINT) ; 
            } 
        
        else 
            { 
            UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 135 );
            
            goto S2_CresNode_Exit__CMDPARSER ; 
            } 
        
        } 
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 139 );
    if ( ((CompareStrings( LOCAL_STRING_STRUCT( __SERVICE  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_4__ ) , 1 ) == 0) && (CompareStrings( LOCAL_STRING_STRUCT( __CMD  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_2__ ) , 1 ) == 0))) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 140 );
        if( ObtainStringOutputSemaphore( INSTANCE_PTR( S2_CresNode ) ) == 0 ) {
        FormatString ( INSTANCE_PTR( S2_CresNode ) , GENERIC_STRING_OUTPUT( S2_CresNode )  ,2 , "%s"  , LOCAL_STRING_STRUCT( __VALUE  )   )  ; 
        SET_STRING_OUT_ARRAY_ELEMENT( S2_CresNode, __STRING_SET , __IDINT ) ; 
        ReleaseStringOutputSemaphore( INSTANCE_PTR( S2_CresNode ) ); }
        
        ; 
        } 
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 149 );
    if ( ((CompareStrings( LOCAL_STRING_STRUCT( __SERVICE  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ ) , 1 ) == 0) && (CompareStrings( LOCAL_STRING_STRUCT( __CMD  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_5__ ) , 1 ) == 0))) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 150 );
        S2_CresNode__CMDBUILDER (  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ )   ,  (struct StringHdr_s* )  LOCAL_STRING_STRUCT( __ID  )  ,  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_6__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , GetInOutArrayElement( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__DIGITAL_VALUE , __IDINT  )) ) ; 
        } 
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 152 );
    if ( ((CompareStrings( LOCAL_STRING_STRUCT( __SERVICE  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_3__ ) , 1 ) == 0) && (CompareStrings( LOCAL_STRING_STRUCT( __CMD  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_5__ ) , 1 ) == 0))) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 153 );
        S2_CresNode__CMDBUILDER (  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_3__ )   ,  (struct StringHdr_s* )  LOCAL_STRING_STRUCT( __ID  )  ,  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_6__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , GetInOutArrayElement( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__ANALOG_VALUE , __IDINT  )) ) ; 
        } 
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 155 );
    if ( ((CompareStrings( LOCAL_STRING_STRUCT( __SERVICE  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_4__ ) , 1 ) == 0) && (CompareStrings( LOCAL_STRING_STRUCT( __CMD  ) , LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_5__ ) , 1 ) == 0))) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 156 );
        S2_CresNode__CMDBUILDER (  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_4__ )   ,  (struct StringHdr_s* )  LOCAL_STRING_STRUCT( __ID  )  ,  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_6__ )   ,  (struct StringHdr_s* )  GetStringArrayElement ( INSTANCE_PTR( S2_CresNode ),  GLOBAL_STRING_ARRAY( S2_CresNode, __STRING_VALUE  )    ,  __IDINT  ) ) ; 
        } 
    
    
    S2_CresNode_Exit__CMDPARSER:
/* Begin Free local function variables */
    FREE_LOCAL_STRING_STRUCT( __SERVICE );
    FREE_LOCAL_STRING_STRUCT( __ID );
    FREE_LOCAL_STRING_STRUCT( __CMD );
    FREE_LOCAL_STRING_STRUCT( __VALUE );
    FREE_LOCAL_STRING_STRUCT( __RETURNVALUE$ );
    FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ );
    FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__1 );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_2__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_3__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_4__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_5__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_6__ );
    /* End Free local function variables */
    
    }
    
static void S2_CresNode__SOCKETRX ( ) 
    { 
    /* Begin local function variable declarations */
    
    CREATE_STRING_STRUCT( S2_CresNode, __TEMP$, 65000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __TEMP$ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__, sizeof( "*" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 65000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1, 65000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __TEMP$ );
    INITIALIZE_LOCAL_STRING_STRUCT( __TEMP$, 65000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__, "*" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 65000 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__1, 65000 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 166 );
    while ( (Len( GLOBAL_STRING_STRUCT( S2_CresNode, __INTERNALRXBUFFER  )  ) > 1)) 
        { 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 167 );
        FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Remove ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,  LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   , GLOBAL_STRING_STRUCT( S2_CresNode, __INTERNALRXBUFFER  )    , 1  )  )  ; 
        FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __TEMP$  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 168 );
        FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    ,2 , "%s"  , Left ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , LOCAL_STRING_STRUCT( __TEMP$  )  , (Len( LOCAL_STRING_STRUCT( __TEMP$  )  ) - 1))  )  ; 
        FormatString ( INSTANCE_PTR( S2_CresNode ) , LOCAL_STRING_STRUCT( __TEMP$  )   ,2 , "%s"  , __FN_DST_STR__1 ) ; 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 169 );
        S2_CresNode__CMDPARSER (  (struct StringHdr_s* )  LOCAL_STRING_STRUCT( __TEMP$  )  ) ; 
        UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 166 );
        } 
    
    
    S2_CresNode_Exit__SOCKETRX:
/* Begin Free local function variables */
    FREE_LOCAL_STRING_STRUCT( __TEMP$ );
    FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
    FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ );
    FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__1 );
    /* End Free local function variables */
    
    }
    
DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00000 /*Digital_Value*/ )

    {
    /* Begin local function variable declarations */
    
    unsigned short  __ID; 
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__, sizeof( "Digital" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__, sizeof( "Event" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 50 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1, 50 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    
    SAVE_GLOBAL_POINTERS ;
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__, "Digital" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__, "Event" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 50 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__1, 50 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 179 );
    __ID = GetLocalLastModifiedArrayIndex ( S2_CresNode ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 180 );
    S2_CresNode__CMDBUILDER (  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , __ID) ,  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    , GetInOutArrayElement( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__DIGITAL_VALUE , __ID  )) ) ; 
    
    S2_CresNode_Exit__Event_0:
    /* Begin Free local function variables */
FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ );
FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ );
FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__1 );
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}

DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00001 /*Analog_Value*/ )

    {
    /* Begin local function variable declarations */
    
    unsigned short  __ID; 
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__, sizeof( "Analog" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__, sizeof( "Event" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 50 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1, 50 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    
    SAVE_GLOBAL_POINTERS ;
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__, "Analog" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__, "Event" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 50 );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__1 );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__1, 50 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 184 );
    __ID = GetLocalLastModifiedArrayIndex ( S2_CresNode ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 185 );
    S2_CresNode__CMDBUILDER (  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , __ID) ,  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__1 )    , GetInOutArrayElement( INSTANCE_PTR( S2_CresNode ), &Globals->S2_CresNode.__ANALOG_VALUE , __ID  )) ) ; 
    
    S2_CresNode_Exit__Event_1:
    /* Begin Free local function variables */
FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ );
FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ );
FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__1 );
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}

DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00002 /*String_Value*/ )

    {
    /* Begin local function variable declarations */
    
    unsigned short  __ID; 
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__, sizeof( "String" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__, sizeof( "Event" ) );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 50 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    SAVE_GLOBAL_POINTERS ;
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_0__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__, "String" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __SPLS_TMPVAR__LOCALSTR_1__ );
    SET_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__, "Event" );
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 50 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 190 );
    __ID = GetLocalLastModifiedArrayIndex ( S2_CresNode ); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 191 );
    S2_CresNode__CMDBUILDER (  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ )   ,  (struct StringHdr_s* )  Ltoa ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    , __ID) ,  (struct StringHdr_s* )   LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ )   ,  (struct StringHdr_s* )  GetStringArrayElement ( INSTANCE_PTR( S2_CresNode ),  GLOBAL_STRING_ARRAY( S2_CresNode, __STRING_VALUE  )    ,  __ID  ) ) ; 
    
    S2_CresNode_Exit__Event_2:
    /* Begin Free local function variables */
FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_0__ );
FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
FREE_LOCAL_STRING_STRUCT( __SPLS_TMPVAR__LOCALSTR_1__ );
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}

DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00003 /*MyServer*/ )

    {
    /* Begin local function variable declarations */
    
    SAVE_GLOBAL_POINTERS ;
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 199 );
    Globals->S2_CresNode.__DOSERVERCONNECTED = 1; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 200 );
    Print( INSTANCE_PTR( S2_CresNode ) , 37, "OnConnect: input buffer size is: %d\r\n", (short)( Len( (struct StringHdr_s* )& GLOBAL_STRUCT_FIELD( S2_CresNode, __MYSERVER, SocketRxBuf ) ) )) ; 
    
    S2_CresNode_Exit__Event_3:
    /* Begin Free local function variables */
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}

DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00004 /*MyServer*/ )

    {
    /* Begin local function variable declarations */
    
    SAVE_GLOBAL_POINTERS ;
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 204 );
    Globals->S2_CresNode.__DOSERVERCONNECTED = 0; 
    
    S2_CresNode_Exit__Event_4:
    /* Begin Free local function variables */
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}

DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00005 /*MyServer*/ )

    {
    /* Begin local function variable declarations */
    
    short  __STATUS; 
    SAVE_GLOBAL_POINTERS ;
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 209 );
    __STATUS = SocketGetStatus(); 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 211 );
    Print( INSTANCE_PTR( S2_CresNode ) , 39, "The SocketGetStatus returns:       %d\r\n", (short)( __STATUS )) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 212 );
    Print( INSTANCE_PTR( S2_CresNode ) , 39, "The MyServer.SocketStatus returns: %d\r\n", (short)( GLOBAL_STRUCT_FIELD( S2_CresNode, __MYSERVER,SocketStatus ) )) ; 
    
    S2_CresNode_Exit__Event_5:
    /* Begin Free local function variables */
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}

DEFINE_INDEPENDENT_EVENTHANDLER( S2_CresNode, 00006 /*MyServer*/ )

    {
    /* Begin local function variable declarations */
    
    CREATE_STRING_STRUCT( S2_CresNode, __FN_DST_STR__, 65000 );
    DECLARE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    
    SAVE_GLOBAL_POINTERS ;
    
    ALLOCATE_LOCAL_STRING_STRUCT( S2_CresNode, __FN_DST_STR__ );
    INITIALIZE_LOCAL_STRING_STRUCT( __FN_DST_STR__, 65000 );
    
    
    /* End local function variable declarations */
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 216 );
    FormatString ( INSTANCE_PTR( S2_CresNode ) ,  LOCAL_STRING_STRUCT( __FN_DST_STR__ )    ,4 , "%s%s"  , GLOBAL_STRING_STRUCT( S2_CresNode, __INTERNALRXBUFFER  )  ,    & GLOBAL_STRUCT_FIELD( S2_CresNode, __MYSERVER, SocketRxBuf ) )  ; 
    FormatString ( INSTANCE_PTR( S2_CresNode ) , GLOBAL_STRING_STRUCT( S2_CresNode, __INTERNALRXBUFFER  )   ,2 , "%s"  , __FN_DST_STR__ ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 217 );
    ClearBuffer ( (struct StringHdr_s* )& GLOBAL_STRUCT_FIELD( S2_CresNode, __MYSERVER, SocketRxBuf )) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 218 );
    S2_CresNode__SOCKETRX ( ) ; 
    
    S2_CresNode_Exit__Event_6:
    /* Begin Free local function variables */
FREE_LOCAL_STRING_STRUCT( __FN_DST_STR__ );
/* End Free local function variables */
RESTORE_GLOBAL_POINTERS ;

}


    
    

/********************************************************************************
  Constructor
********************************************************************************/

/********************************************************************************
  Destructor
********************************************************************************/

/********************************************************************************
  static void ProcessDigitalEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessDigitalEvent( struct Signal_s *Signal )
{
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_DIGITAL_VALUE_DIG_INPUT :
            if ( Signal->Value /*Push*/ )
            {
                
            }
            else /*Release*/
            {
                
            }
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00000 /*Digital_Value*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  static void ProcessAnalogEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessAnalogEvent( struct Signal_s *Signal )
{
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_ANALOG_VALUE_ANALOG_INPUT :
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00001 /*Analog_Value*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  static void ProcessStringEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessStringEvent( struct Signal_s *Signal )
{
    if ( UPDATE_INPUT_STRING( S2_CresNode ) == 1 ) return ;
    
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_STRING_VALUE_STRING_INPUT :
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00002 /*String_Value*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  static void ProcessSocketConnectEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessSocketConnectEvent( struct Signal_s *Signal )
{
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_MYSERVER_SOCKET :
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00003 /*MyServer*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  static void ProcessSocketDisconnectEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessSocketDisconnectEvent( struct Signal_s *Signal )
{
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_MYSERVER_SOCKET :
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00004 /*MyServer*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  static void ProcessSocketReceiveEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessSocketReceiveEvent( struct Signal_s *Signal )
{
    if ( UPDATE_INPUT_STRING( S2_CresNode ) == 1 ) return ;
    
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_MYSERVER_SOCKET :
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00006 /*MyServer*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  static void ProcessSocketStatusEvent( struct Signal_s *Signal )
********************************************************************************/
static void ProcessSocketStatusEvent( struct Signal_s *Signal )
{
    switch ( Signal->SignalNumber )
    {
        case __S2_CresNode_MYSERVER_SOCKET :
            SAFESPAWN_EVENTHANDLER( S2_CresNode, 00005 /*MyServer*/, 0 );
            break;
            
        default :
            SetSymbolEventStart ( INSTANCE_PTR( S2_CresNode ) ); 
            break ;
        
    }
}

/********************************************************************************
  EVENT_HANDLER( S2_CresNode )
********************************************************************************/
EVENT_HANDLER( S2_CresNode )
    {
    SAVE_GLOBAL_POINTERS ;
    CHECK_INPUT_ARRAY ( S2_CresNode, __DIGITAL_VALUE ) ;
    CHECK_INPUT_ARRAY ( S2_CresNode, __ANALOG_VALUE ) ;
    CHECK_STRING_INPUT_ARRAY ( S2_CresNode, __STRING_VALUE ) ;
    switch ( Signal->Type )
        {
        case e_SIGNAL_TYPE_DIGITAL :
            ProcessDigitalEvent( Signal );
            break ;
        case e_SIGNAL_TYPE_ANALOG :
            ProcessAnalogEvent( Signal );
            break ;
        case e_SIGNAL_TYPE_STRING :
            ProcessStringEvent( Signal );
            break ;
        case e_SIGNAL_TYPE_CONNECT :
            ProcessSocketConnectEvent( Signal );
            break ;
        case e_SIGNAL_TYPE_DISCONNECT :
            ProcessSocketDisconnectEvent( Signal );
            break ;
        case e_SIGNAL_TYPE_RECEIVE :
            ProcessSocketReceiveEvent( Signal );
            break ;
        case e_SIGNAL_TYPE_STATUS :
            ProcessSocketStatusEvent( Signal );
            break ;
        }
        
    RESTORE_GLOBAL_POINTERS ;
    
    }
    
/********************************************************************************
  FUNCTION_MAIN( S2_CresNode )
********************************************************************************/
FUNCTION_MAIN( S2_CresNode )
{
    /* Begin local function variable declarations */
    
    SAVE_GLOBAL_POINTERS ;
    SET_INSTANCE_POINTER ( S2_CresNode );
    
    
    /* End local function variable declarations */
    
    INITIALIZE_IO_ARRAY ( S2_CresNode, __DIGITAL_VALUE, IO_TYPE_DIGITAL_INPUT, __S2_CresNode_DIGITAL_VALUE_DIG_INPUT, __S2_CresNode_DIGITAL_VALUE_ARRAY_LENGTH );
    INITIALIZE_IO_ARRAY ( S2_CresNode, __DIGITAL_TRUE, IO_TYPE_DIGITAL_OUTPUT, __S2_CresNode_DIGITAL_TRUE_DIG_OUTPUT, __S2_CresNode_DIGITAL_TRUE_ARRAY_LENGTH );
    INITIALIZE_IO_ARRAY ( S2_CresNode, __DIGITAL_FALSE, IO_TYPE_DIGITAL_OUTPUT, __S2_CresNode_DIGITAL_FALSE_DIG_OUTPUT, __S2_CresNode_DIGITAL_FALSE_ARRAY_LENGTH );
    INITIALIZE_IO_ARRAY ( S2_CresNode, __ANALOG_VALUE, IO_TYPE_ANALOG_INPUT, __S2_CresNode_ANALOG_VALUE_ANALOG_INPUT, __S2_CresNode_ANALOG_VALUE_ARRAY_LENGTH );
    INITIALIZE_IO_ARRAY ( S2_CresNode, __ANALOG_SET, IO_TYPE_ANALOG_OUTPUT, __S2_CresNode_ANALOG_SET_ANALOG_OUTPUT, __S2_CresNode_ANALOG_SET_ARRAY_LENGTH );
    INITIALIZE_IO_ARRAY ( S2_CresNode, __STRING_SET, IO_TYPE_STRING_OUTPUT, __S2_CresNode_STRING_SET_STRING_OUTPUT, __S2_CresNode_STRING_SET_ARRAY_LENGTH );
    
    INITIALIZE_GLOBAL_STRING_INPUT_ARRAY( S2_CresNode, __STRING_VALUE, e_INPUT_TYPE_STRING, __S2_CresNode_STRING_VALUE_ARRAY_NUM_ELEMS, __S2_CresNode_STRING_VALUE_ARRAY_NUM_CHARS, __S2_CresNode_STRING_VALUE_STRING_INPUT );
    REGISTER_GLOBAL_INPUT_STRING_ARRAY ( S2_CresNode, __STRING_VALUE );
    INITIALIZE_TCP_SERVER( S2_CresNode, __MYSERVER, __S2_CresNode_MYSERVER_STRING_MAX_LEN, __S2_CresNode_MYSERVER_SOCKET );
    
    InitStringParamStruct ( INSTANCE_PTR( S2_CresNode ), GLOBAL_STRING_STRUCT( S2_CresNode, __IPADDR$ ) , e_INPUT_TYPE_STRING_PARAMETER, __S2_CresNode_IPADDR$_STRING_PARAMETER, __S2_CresNode_IPADDR$_PARAM_MAX_LEN, NAME_AS_STRING( __IPADDR$ ) );
    INITIALIZE_GLOBAL_STRING_STRUCT ( S2_CresNode, __INTERNALRXBUFFER, e_INPUT_TYPE_NONE, __S2_CresNode_INTERNALRXBUFFER_STRING_MAX_LEN );
    INITIALIZE_GLOBAL_STRING_STRUCT ( S2_CresNode, sGenericOutStr, e_INPUT_TYPE_NONE, GENERIC_STRING_OUTPUT_LEN );
    
    
    
    
    
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 224 );
    WaitForInitializationComplete ( INSTANCE_PTR( S2_CresNode ) ) ; 
    UpdateSourceCodeLine( INSTANCE_PTR( S2_CresNode ), 226 );
    S2_CresNode__STARTSERVER ( ) ; 
    
    S2_CresNode_Exit__MAIN:/* Begin Free local function variables */
    /* End Free local function variables */
    
    RESTORE_GLOBAL_POINTERS ;
    return 0 ;
    }

