using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Linq;
using Crestron;
using Crestron.Logos.SplusLibrary;
using Crestron.Logos.SplusObjects;
using Crestron.SimplSharp;

namespace UserModule_CRESNODE
{
    public class UserModuleClass_CRESNODE : SplusObject
    {
        static CCriticalSection g_criticalSection = new CCriticalSection();
        
        
        
        
        
        StringParameter IPADDR__DOLLAR__;
        UShortParameter PORT;
        InOutArray<Crestron.Logos.SplusObjects.DigitalInput> DIGITAL_VALUE;
        InOutArray<Crestron.Logos.SplusObjects.DigitalOutput> DIGITAL_TRUE;
        InOutArray<Crestron.Logos.SplusObjects.DigitalOutput> DIGITAL_FALSE;
        InOutArray<Crestron.Logos.SplusObjects.AnalogInput> ANALOG_VALUE;
        InOutArray<Crestron.Logos.SplusObjects.AnalogOutput> ANALOG_SET;
        InOutArray<Crestron.Logos.SplusObjects.StringInput> STRING_VALUE;
        InOutArray<Crestron.Logos.SplusObjects.StringOutput> STRING_SET;
        short DOSERVERCONNECTED = 0;
        CrestronString INTERNALRXBUFFER;
        SplusTcpServer MYSERVER;
        private void STARTSERVER (  SplusExecutionContext __context__ ) 
            { 
            short STATUS = 0;
            
            
            __context__.SourceCodeLine = 57;
            STATUS = (short) ( Functions.SocketServerStartListen( MYSERVER , IPADDR__DOLLAR__  , (ushort)( PORT  .Value ) ) ) ; 
            __context__.SourceCodeLine = 58;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( STATUS < 0 ))  ) ) 
                { 
                __context__.SourceCodeLine = 59;
                Print( "Error listening to {0} on port {1:d} (status: {2:d})", IPADDR__DOLLAR__ , (ushort)PORT  .Value, (short)STATUS) ; 
                } 
            
            else 
                { 
                __context__.SourceCodeLine = 61;
                Print( "Server started to {0} on port {1:d} (status: {2:d})", IPADDR__DOLLAR__ , (ushort)PORT  .Value, (short)STATUS) ; 
                } 
            
            
            }
            
        private void SOCKETTX (  SplusExecutionContext __context__, CrestronString SERVERTX ) 
            { 
            short ISTATUS = 0;
            
            
            __context__.SourceCodeLine = 68;
            ISTATUS = (short) ( Functions.SocketSend( MYSERVER , SERVERTX ) ) ; 
            __context__.SourceCodeLine = 70;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( ISTATUS < 0 ))  ) ) 
                { 
                __context__.SourceCodeLine = 71;
                Print( "Error Sending to MyServer: {0:d}\r\n", (short)ISTATUS) ; 
                } 
            
            else 
                { 
                } 
            
            
            }
            
        private void CMDBUILDER (  SplusExecutionContext __context__, CrestronString SERVICE , CrestronString ID , CrestronString CMD , CrestronString VALUE ) 
            { 
            CrestronString CMDBUILDER__DOLLAR__;
            CMDBUILDER__DOLLAR__  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 1000, this );
            
            
            __context__.SourceCodeLine = 79;
            CMDBUILDER__DOLLAR__  .UpdateValue ( SERVICE + ":" + ID + ":" + CMD + ":" + VALUE + "*"  ) ; 
            __context__.SourceCodeLine = 80;
            SOCKETTX (  __context__ , CMDBUILDER__DOLLAR__) ; 
            
            }
            
        private void CMDPARSER (  SplusExecutionContext __context__, CrestronString SERVERRX ) 
            { 
            CrestronString SERVICE;
            SERVICE  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 1000, this );
            
            CrestronString ID;
            ID  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 1000, this );
            
            CrestronString CMD;
            CMD  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 100, this );
            
            CrestronString VALUE;
            VALUE  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 1000, this );
            
            ushort IDINT = 0;
            ushort VALUEINT = 0;
            
            ushort PULSETIME = 0;
            
            CrestronString RETURNVALUE__DOLLAR__;
            RETURNVALUE__DOLLAR__  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 1000, this );
            
            ushort RETURNVALUE = 0;
            
            ushort TEMP = 0;
            
            
            __context__.SourceCodeLine = 94;
            SERVICE  .UpdateValue ( Functions.Remove ( ":" , SERVERRX )  ) ; 
            __context__.SourceCodeLine = 95;
            SERVICE  .UpdateValue ( Functions.Left ( SERVICE ,  (int) ( (Functions.Length( SERVICE ) - 1) ) )  ) ; 
            __context__.SourceCodeLine = 97;
            ID  .UpdateValue ( Functions.Remove ( ":" , SERVERRX )  ) ; 
            __context__.SourceCodeLine = 98;
            ID  .UpdateValue ( Functions.Left ( ID ,  (int) ( (Functions.Length( ID ) - 1) ) )  ) ; 
            __context__.SourceCodeLine = 99;
            IDINT = (ushort) ( Functions.Atoi( ID ) ) ; 
            __context__.SourceCodeLine = 101;
            CMD  .UpdateValue ( Functions.Remove ( ":" , SERVERRX )  ) ; 
            __context__.SourceCodeLine = 102;
            CMD  .UpdateValue ( Functions.Left ( CMD ,  (int) ( (Functions.Length( CMD ) - 1) ) )  ) ; 
            __context__.SourceCodeLine = 104;
            VALUE  .UpdateValue ( SERVERRX  ) ; 
            __context__.SourceCodeLine = 105;
            VALUEINT = (ushort) ( Functions.Atoi( VALUE ) ) ; 
            __context__.SourceCodeLine = 113;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt (SERVICE == "Digital") ) && Functions.TestForTrue ( Functions.BoolToInt (CMD == "Set") )) ))  ) ) 
                { 
                __context__.SourceCodeLine = 115;
                if ( Functions.TestForTrue  ( ( Functions.BoolToInt (VALUEINT == 1))  ) ) 
                    { 
                    __context__.SourceCodeLine = 116;
                    Functions.Pulse ( 20, DIGITAL_TRUE [ IDINT] ) ; 
                    } 
                
                __context__.SourceCodeLine = 120;
                if ( Functions.TestForTrue  ( ( Functions.BoolToInt (VALUEINT == 0))  ) ) 
                    { 
                    __context__.SourceCodeLine = 121;
                    Functions.Pulse ( 20, DIGITAL_FALSE [ IDINT] ) ; 
                    } 
                
                } 
            
            __context__.SourceCodeLine = 127;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt (SERVICE == "Analog") ) && Functions.TestForTrue ( Functions.BoolToInt (CMD == "Set") )) ))  ) ) 
                { 
                __context__.SourceCodeLine = 130;
                if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt ( VALUEINT <= 65535 ) ) && Functions.TestForTrue ( Functions.BoolToInt ( VALUEINT >= 0 ) )) ))  ) ) 
                    { 
                    __context__.SourceCodeLine = 131;
                    ANALOG_SET [ IDINT]  .Value = (ushort) ( VALUEINT ) ; 
                    } 
                
                else 
                    { 
                    __context__.SourceCodeLine = 135;
                    return ; 
                    } 
                
                } 
            
            __context__.SourceCodeLine = 139;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt (SERVICE == "String") ) && Functions.TestForTrue ( Functions.BoolToInt (CMD == "Set") )) ))  ) ) 
                { 
                __context__.SourceCodeLine = 140;
                STRING_SET [ IDINT]  .UpdateValue ( VALUE  ) ; 
                } 
            
            __context__.SourceCodeLine = 149;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt (SERVICE == "Digital") ) && Functions.TestForTrue ( Functions.BoolToInt (CMD == "get") )) ))  ) ) 
                { 
                __context__.SourceCodeLine = 150;
                CMDBUILDER (  __context__ , "Digital", ID, "Event", Functions.LtoA( (int)( DIGITAL_VALUE[ IDINT ] .Value ) )) ; 
                } 
            
            __context__.SourceCodeLine = 152;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt (SERVICE == "Analog") ) && Functions.TestForTrue ( Functions.BoolToInt (CMD == "get") )) ))  ) ) 
                { 
                __context__.SourceCodeLine = 153;
                CMDBUILDER (  __context__ , "Analog", ID, "Event", Functions.LtoA( (int)( ANALOG_VALUE[ IDINT ] .UshortValue ) )) ; 
                } 
            
            __context__.SourceCodeLine = 155;
            if ( Functions.TestForTrue  ( ( Functions.BoolToInt ( (Functions.TestForTrue ( Functions.BoolToInt (SERVICE == "String") ) && Functions.TestForTrue ( Functions.BoolToInt (CMD == "get") )) ))  ) ) 
                { 
                __context__.SourceCodeLine = 156;
                CMDBUILDER (  __context__ , "String", ID, "Event", STRING_VALUE[ IDINT ]) ; 
                } 
            
            
            }
            
        private void SOCKETRX (  SplusExecutionContext __context__ ) 
            { 
            CrestronString TEMP__DOLLAR__;
            TEMP__DOLLAR__  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 65000, this );
            
            
            __context__.SourceCodeLine = 166;
            while ( Functions.TestForTrue  ( ( Functions.BoolToInt ( Functions.Length( INTERNALRXBUFFER ) > 1 ))  ) ) 
                { 
                __context__.SourceCodeLine = 167;
                TEMP__DOLLAR__  .UpdateValue ( Functions.Remove ( "*" , INTERNALRXBUFFER )  ) ; 
                __context__.SourceCodeLine = 168;
                TEMP__DOLLAR__  .UpdateValue ( Functions.Left ( TEMP__DOLLAR__ ,  (int) ( (Functions.Length( TEMP__DOLLAR__ ) - 1) ) )  ) ; 
                __context__.SourceCodeLine = 169;
                CMDPARSER (  __context__ , TEMP__DOLLAR__) ; 
                __context__.SourceCodeLine = 166;
                } 
            
            
            }
            
        object DIGITAL_VALUE_OnChange_0 ( Object __EventInfo__ )
        
            { 
            Crestron.Logos.SplusObjects.SignalEventArgs __SignalEventArg__ = (Crestron.Logos.SplusObjects.SignalEventArgs)__EventInfo__;
            try
            {
                SplusExecutionContext __context__ = SplusThreadStartCode(__SignalEventArg__);
                ushort ID = 0;
                
                
                __context__.SourceCodeLine = 179;
                ID = (ushort) ( Functions.GetLastModifiedArrayIndex( __SignalEventArg__ ) ) ; 
                __context__.SourceCodeLine = 180;
                CMDBUILDER (  __context__ , "Digital", Functions.LtoA( (int)( ID ) ), "Event", Functions.LtoA( (int)( DIGITAL_VALUE[ ID ] .Value ) )) ; 
                
                
            }
            catch(Exception e) { ObjectCatchHandler(e); }
            finally { ObjectFinallyHandler( __SignalEventArg__ ); }
            return this;
            
        }
        
    object ANALOG_VALUE_OnChange_1 ( Object __EventInfo__ )
    
        { 
        Crestron.Logos.SplusObjects.SignalEventArgs __SignalEventArg__ = (Crestron.Logos.SplusObjects.SignalEventArgs)__EventInfo__;
        try
        {
            SplusExecutionContext __context__ = SplusThreadStartCode(__SignalEventArg__);
            ushort ID = 0;
            
            
            __context__.SourceCodeLine = 184;
            ID = (ushort) ( Functions.GetLastModifiedArrayIndex( __SignalEventArg__ ) ) ; 
            __context__.SourceCodeLine = 185;
            CMDBUILDER (  __context__ , "Analog", Functions.LtoA( (int)( ID ) ), "Event", Functions.LtoA( (int)( ANALOG_VALUE[ ID ] .UshortValue ) )) ; 
            
            
        }
        catch(Exception e) { ObjectCatchHandler(e); }
        finally { ObjectFinallyHandler( __SignalEventArg__ ); }
        return this;
        
    }
    
object STRING_VALUE_OnChange_2 ( Object __EventInfo__ )

    { 
    Crestron.Logos.SplusObjects.SignalEventArgs __SignalEventArg__ = (Crestron.Logos.SplusObjects.SignalEventArgs)__EventInfo__;
    try
    {
        SplusExecutionContext __context__ = SplusThreadStartCode(__SignalEventArg__);
        ushort ID = 0;
        
        
        __context__.SourceCodeLine = 190;
        ID = (ushort) ( Functions.GetLastModifiedArrayIndex( __SignalEventArg__ ) ) ; 
        __context__.SourceCodeLine = 191;
        CMDBUILDER (  __context__ , "String", Functions.LtoA( (int)( ID ) ), "Event", STRING_VALUE[ ID ]) ; 
        
        
    }
    catch(Exception e) { ObjectCatchHandler(e); }
    finally { ObjectFinallyHandler( __SignalEventArg__ ); }
    return this;
    
}

object MYSERVER_OnSocketConnect_3 ( Object __Info__ )

    { 
    SocketEventInfo __SocketInfo__ = (SocketEventInfo)__Info__;
    try
    {
        SplusExecutionContext __context__ = SplusThreadStartCode(__SocketInfo__);
        
        __context__.SourceCodeLine = 199;
        DOSERVERCONNECTED = (short) ( 1 ) ; 
        __context__.SourceCodeLine = 200;
        Print( "OnConnect: input buffer size is: {0:d}\r\n", (short)Functions.Length( MYSERVER.SocketRxBuf )) ; 
        
        
    }
    catch(Exception e) { ObjectCatchHandler(e); }
    finally { ObjectFinallyHandler( __SocketInfo__ ); }
    return this;
    
}

object MYSERVER_OnSocketDisconnect_4 ( Object __Info__ )

    { 
    SocketEventInfo __SocketInfo__ = (SocketEventInfo)__Info__;
    try
    {
        SplusExecutionContext __context__ = SplusThreadStartCode(__SocketInfo__);
        
        __context__.SourceCodeLine = 204;
        DOSERVERCONNECTED = (short) ( 0 ) ; 
        
        
    }
    catch(Exception e) { ObjectCatchHandler(e); }
    finally { ObjectFinallyHandler( __SocketInfo__ ); }
    return this;
    
}

object MYSERVER_OnSocketStatus_5 ( Object __Info__ )

    { 
    SocketEventInfo __SocketInfo__ = (SocketEventInfo)__Info__;
    try
    {
        SplusExecutionContext __context__ = SplusThreadStartCode(__SocketInfo__);
        short STATUS = 0;
        
        
        __context__.SourceCodeLine = 209;
        STATUS = (short) ( __SocketInfo__.SocketStatus ) ; 
        __context__.SourceCodeLine = 211;
        Print( "The SocketGetStatus returns:       {0:d}\r\n", (short)STATUS) ; 
        __context__.SourceCodeLine = 212;
        Print( "The MyServer.SocketStatus returns: {0:d}\r\n", (short)MYSERVER.SocketStatus) ; 
        
        
    }
    catch(Exception e) { ObjectCatchHandler(e); }
    finally { ObjectFinallyHandler( __SocketInfo__ ); }
    return this;
    
}

object MYSERVER_OnSocketReceive_6 ( Object __Info__ )

    { 
    SocketEventInfo __SocketInfo__ = (SocketEventInfo)__Info__;
    try
    {
        SplusExecutionContext __context__ = SplusThreadStartCode(__SocketInfo__);
        
        __context__.SourceCodeLine = 216;
        INTERNALRXBUFFER  .UpdateValue ( INTERNALRXBUFFER + MYSERVER .  SocketRxBuf  ) ; 
        __context__.SourceCodeLine = 217;
        Functions.ClearBuffer ( MYSERVER .  SocketRxBuf ) ; 
        __context__.SourceCodeLine = 218;
        SOCKETRX (  __context__  ) ; 
        
        
    }
    catch(Exception e) { ObjectCatchHandler(e); }
    finally { ObjectFinallyHandler( __SocketInfo__ ); }
    return this;
    
}

public override object FunctionMain (  object __obj__ ) 
    { 
    try
    {
        SplusExecutionContext __context__ = SplusFunctionMainStartCode();
        
        __context__.SourceCodeLine = 224;
        WaitForInitializationComplete ( ) ; 
        __context__.SourceCodeLine = 226;
        STARTSERVER (  __context__  ) ; 
        
        
    }
    catch(Exception e) { ObjectCatchHandler(e); }
    finally { ObjectFinallyHandler(); }
    return __obj__;
    }
    

public override void LogosSplusInitialize()
{
    SocketInfo __socketinfo__ = new SocketInfo( 1, this );
    InitialParametersClass.ResolveHostName = __socketinfo__.ResolveHostName;
    _SplusNVRAM = new SplusNVRAM( this );
    INTERNALRXBUFFER  = new CrestronString( Crestron.Logos.SplusObjects.CrestronStringEncoding.eEncodingASCII, 65000, this );
    MYSERVER  = new SplusTcpServer ( 9999, this );
    
    DIGITAL_VALUE = new InOutArray<DigitalInput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        DIGITAL_VALUE[i+1] = new Crestron.Logos.SplusObjects.DigitalInput( DIGITAL_VALUE__DigitalInput__ + i, DIGITAL_VALUE__DigitalInput__, this );
        m_DigitalInputList.Add( DIGITAL_VALUE__DigitalInput__ + i, DIGITAL_VALUE[i+1] );
    }
    
    DIGITAL_TRUE = new InOutArray<DigitalOutput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        DIGITAL_TRUE[i+1] = new Crestron.Logos.SplusObjects.DigitalOutput( DIGITAL_TRUE__DigitalOutput__ + i, this );
        m_DigitalOutputList.Add( DIGITAL_TRUE__DigitalOutput__ + i, DIGITAL_TRUE[i+1] );
    }
    
    DIGITAL_FALSE = new InOutArray<DigitalOutput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        DIGITAL_FALSE[i+1] = new Crestron.Logos.SplusObjects.DigitalOutput( DIGITAL_FALSE__DigitalOutput__ + i, this );
        m_DigitalOutputList.Add( DIGITAL_FALSE__DigitalOutput__ + i, DIGITAL_FALSE[i+1] );
    }
    
    ANALOG_VALUE = new InOutArray<AnalogInput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        ANALOG_VALUE[i+1] = new Crestron.Logos.SplusObjects.AnalogInput( ANALOG_VALUE__AnalogSerialInput__ + i, ANALOG_VALUE__AnalogSerialInput__, this );
        m_AnalogInputList.Add( ANALOG_VALUE__AnalogSerialInput__ + i, ANALOG_VALUE[i+1] );
    }
    
    ANALOG_SET = new InOutArray<AnalogOutput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        ANALOG_SET[i+1] = new Crestron.Logos.SplusObjects.AnalogOutput( ANALOG_SET__AnalogSerialOutput__ + i, this );
        m_AnalogOutputList.Add( ANALOG_SET__AnalogSerialOutput__ + i, ANALOG_SET[i+1] );
    }
    
    STRING_VALUE = new InOutArray<StringInput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        STRING_VALUE[i+1] = new Crestron.Logos.SplusObjects.StringInput( STRING_VALUE__AnalogSerialInput__ + i, STRING_VALUE__AnalogSerialInput__, 9999, this );
        m_StringInputList.Add( STRING_VALUE__AnalogSerialInput__ + i, STRING_VALUE[i+1] );
    }
    
    STRING_SET = new InOutArray<StringOutput>( 20, this );
    for( uint i = 0; i < 20; i++ )
    {
        STRING_SET[i+1] = new Crestron.Logos.SplusObjects.StringOutput( STRING_SET__AnalogSerialOutput__ + i, this );
        m_StringOutputList.Add( STRING_SET__AnalogSerialOutput__ + i, STRING_SET[i+1] );
    }
    
    PORT = new UShortParameter( PORT__Parameter__, this );
    m_ParameterList.Add( PORT__Parameter__, PORT );
    
    IPADDR__DOLLAR__ = new StringParameter( IPADDR__DOLLAR____Parameter__, this );
    m_ParameterList.Add( IPADDR__DOLLAR____Parameter__, IPADDR__DOLLAR__ );
    
    
    for( uint i = 0; i < 20; i++ )
        DIGITAL_VALUE[i+1].OnDigitalChange.Add( new InputChangeHandlerWrapper( DIGITAL_VALUE_OnChange_0, false ) );
        
    for( uint i = 0; i < 20; i++ )
        ANALOG_VALUE[i+1].OnAnalogChange.Add( new InputChangeHandlerWrapper( ANALOG_VALUE_OnChange_1, false ) );
        
    for( uint i = 0; i < 20; i++ )
        STRING_VALUE[i+1].OnSerialChange.Add( new InputChangeHandlerWrapper( STRING_VALUE_OnChange_2, false ) );
        
    MYSERVER.OnSocketConnect.Add( new SocketHandlerWrapper( MYSERVER_OnSocketConnect_3, false ) );
    MYSERVER.OnSocketDisconnect.Add( new SocketHandlerWrapper( MYSERVER_OnSocketDisconnect_4, false ) );
    MYSERVER.OnSocketStatus.Add( new SocketHandlerWrapper( MYSERVER_OnSocketStatus_5, false ) );
    MYSERVER.OnSocketReceive.Add( new SocketHandlerWrapper( MYSERVER_OnSocketReceive_6, false ) );
    
    _SplusNVRAM.PopulateCustomAttributeList( true );
    
    NVRAM = _SplusNVRAM;
    
}

public override void LogosSimplSharpInitialize()
{
    
    
}

public UserModuleClass_CRESNODE ( string InstanceName, string ReferenceID, Crestron.Logos.SplusObjects.CrestronStringEncoding nEncodingType ) : base( InstanceName, ReferenceID, nEncodingType ) {}




const uint IPADDR__DOLLAR____Parameter__ = 10;
const uint PORT__Parameter__ = 11;
const uint DIGITAL_VALUE__DigitalInput__ = 0;
const uint DIGITAL_TRUE__DigitalOutput__ = 0;
const uint DIGITAL_FALSE__DigitalOutput__ = 20;
const uint ANALOG_VALUE__AnalogSerialInput__ = 0;
const uint ANALOG_SET__AnalogSerialOutput__ = 0;
const uint STRING_VALUE__AnalogSerialInput__ = 20;
const uint STRING_SET__AnalogSerialOutput__ = 20;

[SplusStructAttribute(-1, true, false)]
public class SplusNVRAM : SplusStructureBase
{

    public SplusNVRAM( SplusObject __caller__ ) : base( __caller__ ) {}
    
    
}

SplusNVRAM _SplusNVRAM = null;

public class __CEvent__ : CEvent
{
    public __CEvent__() {}
    public void Close() { base.Close(); }
    public int Reset() { return base.Reset() ? 1 : 0; }
    public int Set() { return base.Set() ? 1 : 0; }
    public int Wait( int timeOutInMs ) { return base.Wait( timeOutInMs ) ? 1 : 0; }
}
public class __CMutex__ : CMutex
{
    public __CMutex__() {}
    public void Close() { base.Close(); }
    public void ReleaseMutex() { base.ReleaseMutex(); }
    public int WaitForMutex() { return base.WaitForMutex() ? 1 : 0; }
}
 public int IsNull( object obj ){ return (obj == null) ? 1 : 0; }
}


}
