/***************************************************************************************************** 
                                     Includes/Imports 
*****************************************************************************************************/
#include <SPI.h>
#include <Ethernet.h>

/***************************************************************************************************** 
                                      Defines 
*****************************************************************************************************/
/* relay states - these relays are "active low" */
#define RELAY_OFF  ( 1u )
#define RELAY_ON   ( 0u )

/* arduino I/O pin numbers that relays are connected to */
#define RELAY_1  ( 30u )
#define RELAY_2  ( 31u )
#define RELAY_3  ( 32u )
#define RELAY_4  ( 33u )

/* serial comms parameters */
#define SERIAL_0_BAUD_RATE    ( 9600u )  /* PC to Arduino via USB */

/* ethernet parameters */
//TODO: find out why the ethernet libraries don't like these defined as const
static byte mac_addr_s[] = { 0xDEu, 0xADu, 0xBEu, 0xEFu, 0xFEu, 0xEDu };

static IPAddress ip_addr_s( 192u, 168u, 0u, 116u );

#define server_port    ( 80u )    /* default http port */

/***************************************************************************************************** 
                                      Globals 
*****************************************************************************************************/
//const byte ascii_num_zero  = 0x30;
//const byte ascii_num_one   = 0x31;
static byte   input_byte_s;

/***************************************************************************************************** 
                                      Initial setup 
*****************************************************************************************************/
static EthernetServer my_webserver_s( server_port );

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
  Ethernet.begin( mac_addr_s, ip_addr_s );
  
  my_webserver_s.begin();
  
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

/*********************************************************************************
*  \brief   Handles webserver connections, requests and responses
*  \author  A mix of tutorials, with some of my own code to make it a bit safer
*********************************************************************************/
void ethernet_webserver( void )
{  
  EthernetClient client = my_webserver_s.available();
  
  if( client )
  {
    /* a http request ends with a blank line */
    boolean current_line_is_blank = true;
    
    
    /* a client is considered connected if the connection has been closed but 
    there is still unread data */
    // TODO: is there a better way to do this than using a while loop? - potentially blocking
    while( client.connected() )
    {
      //TODO: client.available returns the number of bytes available - use this info!!
      //if( client.available() )   /* TODO: look out for this - if(x) - x could be any number > 0 */
      if( client.available() > 0u )
      {
        char rxd_char = client.read();
        Serial.write( rxd_char );
        
        /* http request ends with a blank line */
        if( rxd_char == '\n' && current_line_is_blank )
        {
           /* send a standard http response header */
           client.println( "HTTP/1.1 200 OK" );
           client.println( "Content-Type: text/html" );
           client.println( "Connection: close" ); /* connection closed when response completes */
           client.println( "Refresh: 15" );        /* auto-refresh the page every 5 seconds */
           client.println();
           
           /* HTML page in response */
           client.println( "<!DOCTYPE HTML>" );
           client.println( "<html>" );
           
           client.println( "YOLO" );  //TODO: add something a bit more wholesome to the response :)
           
           client.println( "</html>" );
        }
        
        if( rxd_char == '\n' )
        {
          current_line_is_blank = true; 
        }
        else if( rxd_char != '\r' )
        {
          current_line_is_blank = false; 
        }
      }   
   
    } 
    
    /* give the web browser time to receive the data */
    delay( 1u );
    
    /* close the connection & notify whoever is listening on the serial port */
    client.stop();
    Serial.println( "client disconnected." );
    
  }
  
}

/*********************************************************************************
*  \brief   Control of Songle relay blocks using input from Serial port
*  \author  EFurlong
*********************************************************************************/
void serial_relay_control( void )
{
  if( Serial.available() )
  {
    /* read the most recent byte received on serial port 0 (from PC via USB) */
    input_byte_s = Serial.read();
    
    /* clear the serial rx buffer - we don't care what comes after first byte */
    Serial.flush();
    
    /* inputs are ASCII - using numbers 1 - 7 to control relays */
    switch( input_byte_s )
    {
      case( 0x30u ):
      {
        Serial.write( "Relay 01 OFF. \n" );
        digitalWrite( RELAY_1, RELAY_OFF );
      }
      break;
     
      case( 0x31u ):
      {
        Serial.write( "Relay 01 ON. \n" );
        digitalWrite( RELAY_1, RELAY_ON );
      }
      break;
    
      case( 0x32u ):
      {
        Serial.write( "Relay 02 OFF. \n" );
        digitalWrite( RELAY_2, RELAY_OFF );
      }
      break;
      
      case( 0x33u ):
      {
        Serial.write( "Relay 02 ON. \n" );
        digitalWrite( RELAY_2, RELAY_ON );
      }
      break;
    
      case( 0x34u ):
      {
        Serial.write( "Relay 03 OFF. \n" );
        digitalWrite( RELAY_3, RELAY_OFF );
      }
      break;
      
      case( 0x35u ):
      {
        Serial.write( "Relay 03 ON. \n" );
        digitalWrite( RELAY_3, RELAY_ON );
      }
      break;
    
      case( 0x36u ):
      {
        Serial.write( "Relay 04 OFF. \n" );
        digitalWrite( RELAY_4, RELAY_OFF );
      }
      break;
      
      case( 0x37u ):
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




