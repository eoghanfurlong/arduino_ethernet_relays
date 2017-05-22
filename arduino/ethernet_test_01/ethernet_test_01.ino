/***************************************************************************************************** 
                                     Includes/Imports 
*****************************************************************************************************/
#include <SPI.h>
#include <Ethernet.h>

/***************************************************************************************************** 
                                      Defines 
*****************************************************************************************************/
/* relay states - these relays are "active low" */
#define RELAY_OFF  ( 1 )
#define RELAY_ON   ( 0 )

/* arduino I/O pin numbers that relays are connected to */
#define RELAY_1  ( 30 )
#define RELAY_2  ( 31 )
#define RELAY_3  ( 32 )
#define RELAY_4  ( 33 )

/* serial comms parameters */
#define SERIAL_0_BAUD_RATE    ( 9600 )  /* PC to Arduino via USB */

/* ethernet parameters */
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip_addr( 192,168,0,116 );

#define server_port    ( 80 )    /* default http port */

/***************************************************************************************************** 
                                      Globals 
*****************************************************************************************************/
//const byte ascii_num_zero  = 0x30;
//const byte ascii_num_one   = 0x31;
static byte   input_byte_s;

/***************************************************************************************************** 
                                      Initial setup 
*****************************************************************************************************/
EthernetServer my_webserver( server_port );

void setup() 
{
  /********** Serial communication setup **********/
  /* start serial comms between arduino and pc via usb */
  Serial.begin( SERIAL_0_BAUD_RATE );
  
  
  /********** Relay setup **********/
  /* ensure that all relays are turned OFF on startup */
  digitalWrite( RELAY_1, RELAY_OFF );
  digitalWrite( RELAY_2, RELAY_OFF );
  digitalWrite( RELAY_3, RELAY_OFF );
  digitalWrite( RELAY_4, RELAY_OFF );
  
  /* set pins connected to relay coils as outputs */
  pinMode( RELAY_1, OUTPUT );
  pinMode( RELAY_2, OUTPUT );
  pinMode( RELAY_3, OUTPUT );
  pinMode( RELAY_4, OUTPUT );
  
  /********** Ethernet setup **********/
  Ethernet.begin( mac_addr, ip_addr );
  
  my_webserver.begin();
  
  Serial.print( "Server is at: " );
  Serial.println( Ethernet.localIP() );
  
}

/***************************************************************************************************** 
                                      Main loop 
*****************************************************************************************************/
void loop() 
{
  /* Relay control over serial bus */
  serial_relay_control();
  
  /* Webserver/ethernet */
  ethernet_webserver();
  
  
}

/***************************************************************************************************** 
                                 Private functions
*****************************************************************************************************/
void ethernet_webserver( void )
{
  EthernetClient client = my_webserver.available();
  
  if( client )
  {
    /* a http request ends with a blank line */
    boolean current_line_is_blank = true;
    
    
    /* a client is considered connected if the connection has been closed but 
    there is still unread data */
    while( client.connected() )
    {
      if( client.available() )
      {
        char rxd_char = client.read();
        Serial.write( rxd_char );
        
        /* http request ends with a blank line */
        if( rxd_char == '\n' )
        {
           /* send a standard http response header */ 
        }
      }   
   
    } 
    
    char c = client.read();
    Serial.write(c);
  }
  
}


void serial_relay_control( void )
{
  if( Serial.available() )
  {
    /* read the most recent byte received on serial port 0 (from PC via USB) */
    input_byte_s = Serial.read();
    
    /* clear the serial rx buffer - we don't care what comes after first byte */
    Serial.flush();
    
    switch( input_byte_s )
    {
      case( 0x30 ):
      {
        Serial.write( "Relay 01 OFF. \n" );
        digitalWrite( RELAY_1, RELAY_OFF );
      }
      break;
     
      case( 0x31 ):
      {
        Serial.write( "Relay 01 ON. \n" );
        digitalWrite( RELAY_1, RELAY_ON );
      }
      break;
    
      case( 0x32 ):
      {
        Serial.write( "Relay 02 OFF. \n" );
        digitalWrite( RELAY_2, RELAY_OFF );
      }
      break;
      
      case( 0x33 ):
      {
        Serial.write( "Relay 02 ON. \n" );
        digitalWrite( RELAY_2, RELAY_ON );
      }
      break;
    
      case( 0x34 ):
      {
        Serial.write( "Relay 03 OFF. \n" );
        digitalWrite( RELAY_3, RELAY_OFF );
      }
      break;
      
      case( 0x35 ):
      {
        Serial.write( "Relay 03 ON. \n" );
        digitalWrite( RELAY_3, RELAY_ON );
      }
      break;
    
      case( 0x36 ):
      {
        Serial.write( "Relay 04 OFF. \n" );
        digitalWrite( RELAY_4, RELAY_OFF );
      }
      break;
      
      case( 0x37 ):
      {
        Serial.write( "Relay 04 ON. \n" );
        digitalWrite( RELAY_4, RELAY_ON );
      }
      break;
     
     default:
     {
       /* do nothing for now */
     }
     break;
      
    }
  }
}




