/***************************************************************************************************** 
                                     Includes/Importa 
*****************************************************************************************************/
/* None */

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

/***************************************************************************************************** 
                                      Globals 
*****************************************************************************************************/
const byte ascii_num_zero  = 0x30;
const byte ascii_num_one   = 0x31;

static byte   input_byte_s;

/***************************************************************************************************** 
                                      Initial setup 
*****************************************************************************************************/
void setup() 
{
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
  
  /* start serial comms between arduino and pc via usb */
  Serial.begin( SERIAL_0_BAUD_RATE );
}

/***************************************************************************************************** 
                                      Main loop 
*****************************************************************************************************/
void loop() 
{
  serial_relay_control();
}

/***************************************************************************************************** 
                                 Private functions
*****************************************************************************************************/
void serial_relay_control()
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
        Serial.write( "number 0 eneterd. \n" );
        digitalWrite( RELAY_1, RELAY_OFF );
      }
      break;
     
      case( 0x31 ):
      {
        Serial.write( "number 1 eneterd. \n" );
        digitalWrite( RELAY_1, RELAY_ON );
      }
      break;
    
      case( 0x32 ):
      {
        Serial.write( "number 2 eneterd. \n" );
        digitalWrite( RELAY_2, RELAY_OFF );
      }
      break;
      
      case( 0x33 ):
      {
        Serial.write( "number 3 eneterd. \n" );
        digitalWrite( RELAY_2, RELAY_ON );
      }
      break;
    
      case( 0x34 ):
      {
        Serial.write( "number 4 eneterd. \n" );
        digitalWrite( RELAY_3, RELAY_OFF );
      }
      break;
      
      case( 0x35 ):
      {
        Serial.write( "number 5 eneterd. \n" );
        digitalWrite( RELAY_3, RELAY_ON );
      }
      break;
    
      case( 0x36 ):
      {
        Serial.write( "number 6 eneterd. \n" );
        digitalWrite( RELAY_4, RELAY_OFF );
      }
      break;
      
      case( 0x37 ):
      {
        Serial.write( "number 7 eneterd. \n" );
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




