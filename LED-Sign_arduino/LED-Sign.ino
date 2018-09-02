#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

// For reference (pin mapping for ESP8266-12E):
// D0/GPIO16      = 16;
// D1/GPIO5       = 5;
// D2/GPIO4       = 0;
// D3/GPIO0       = 4;

// D4/GPIO2       = 2;
// D5/GPIO14      = 14;
// D6/GPIO12      = 13;
// D7/GPIO13      = 12;


// First string of lights. Pins D0 - D3
#define WHITE_LED_FIRST     16
#define BLUE_LED_FIRST      5
#define RED_LED_FIRST       0
#define GREEN_LED_FIRST     4

// Second string of lights. Pins D4 - D7
#define WHITE_LED_SECOND     2
#define BLUE_LED_SECOND      14
#define RED_LED_SECOND       13
#define GREEN_LED_SECOND     12

int maxBrightness = 1024;
int offBrightness = 0;

const char* ssid     = "ostmo";
const char* password = "stikaa11";
int serverPort = 80;
int status = WL_IDLE_STATUS;
IPAddress ip;

WiFiServer server( serverPort  );

int currentR = 0;
int currentG = 0;
int currentB = 0;
int currentW = 0;

int currentRFirst = 0;
int currentGFirst = 0;
int currentBFirst = 0;
int currentWFirst = 0;

int currentRSecond = 0;
int currentGSecond = 0;
int currentBSecond = 0;
int currentWSecond = 0;

void setup() 
{
  Serial.begin( 57600 );
  delay( 100 );

  pinMode( WHITE_LED_FIRST, OUTPUT );
  pinMode( BLUE_LED_FIRST, OUTPUT );
  pinMode( RED_LED_FIRST, OUTPUT );
  pinMode( GREEN_LED_FIRST, OUTPUT );

  pinMode( WHITE_LED_SECOND, OUTPUT );
  pinMode( BLUE_LED_SECOND, OUTPUT );
  pinMode( RED_LED_SECOND, OUTPUT );
  pinMode( GREEN_LED_SECOND, OUTPUT );
  
  connectToWifi();
  initOta();
  server.begin(); // Start the API server/service
}

void turnOn( String whichPosition ) 
{ 
  setColorToLevel( whichPosition, "all", maxBrightness );
}

void turnOff( String whichPosition ) 
{
  setColorToLevel( whichPosition, "all", offBrightness );
}

void setColorToLevel( String whichPosition, String color, int level ) 
{ 

  int redPin = RED_LED_FIRST;
  int greenPin = GREEN_LED_FIRST;
  int bluePin = BLUE_LED_FIRST;
  int whitePin = WHITE_LED_FIRST;

  if( whichPosition == "second" )
  {
    redPin = RED_LED_SECOND;
    greenPin = GREEN_LED_SECOND;
    bluePin = BLUE_LED_SECOND;
    whitePin = WHITE_LED_SECOND;
  }
  
  if( color == "red" )
  {
    analogWrite( redPin, level );
    currentR = level;
  }
  if( color == "green" )
  {
    analogWrite( greenPin, level );
    currentG = level;
  }
  if( color == "blue" )
  {
    analogWrite( bluePin, level );
    currentB = level;
  }
  if( color == "white" )
  {
    analogWrite( whitePin, level );
    currentW = level;
  }
  if( color == "all" )
  {
    analogWrite( redPin, level );
    analogWrite( greenPin, level );
    analogWrite( bluePin, level );
    analogWrite( whitePin, level );
    currentR = level;
    currentG = level;
    currentB = level;
    currentW = level;
  }

  if( whichPosition == "second" )
  {
    currentRSecond = currentR;
    currentGSecond = currentG;
    currentBSecond = currentB;
    currentWSecond = currentW;
  }
  else
  {
    currentRFirst = currentR;
    currentGFirst = currentG;
    currentBFirst = currentB;
    currentWFirst = currentW;
  }
}

void loop() 
{
  ArduinoOTA.handle();
  nodeServer();
}


int parseOtaRequest( String requestString )
{
  char *line;
  char *token;
  char *tokenOut;
  char *finalToken;
  char *currentString = (char *)requestString.c_str();
  String myString = String( currentString );
  
  line = strtok( currentString, "\n" );
  while( line != NULL )
  {
    if( String( line ).indexOf( "?action=otasend" ) > 0 )
    {
      token = tokenOut = strtok( line, "-" );
      while( token != NULL )
      {
        token = strtok( NULL, "-") ;
        if( token != NULL )
        {
          tokenOut = token;
        } 
      }
      break;
    }
    line = strtok( NULL, "\n") ;
  }
  
  if( tokenOut != NULL )
  {
     finalToken = strtok( tokenOut, " " );
  }

  return String( finalToken ).toInt();
}




void connectToWifi()
{
  Serial.print( "Connecting to " );
  Serial.println( ssid );

  WiFi.mode( WIFI_STA );
  WiFi.begin( ssid, password );

  while( WiFi.status() != WL_CONNECTED )
  {
      delay( 500 );
      Serial.print( "." );
  }
  Serial.println( "." );
  Serial.println( "Connected" );
  printWifiStatus();
}

void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
//  Serial.print( "SSID: " );
//  Serial.println( WiFi.SSID() );

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print( "IP Address: " );
  Serial.println( ip );

  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print( "Signal strength (RSSI):" );
//  Serial.print( rssi );
//  Serial.println (" dBm" );
}

void htmlHeader( WiFiClient client )
{
    client.println( "HTTP/1.1 200 OK" );
    client.println ("Content-Type: text/html" );
    client.println( "Connection: close" );  // the connection will be closed after completion of the response
    client.println( "Cache-Control: no-store" );
    client.println();
    client.println( "<!DOCTYPE HTML>" );
    client.println( "<html>" );
}

void htmlFooter( WiFiClient client )
{
    client.println( "</html>" );
}


void initOta()
{
  ArduinoOTA.onStart( []() 
  {
    String type;
    if( ArduinoOTA.getCommand() == U_FLASH )
    {
      type = "sketch";
    }
    else // U_SPIFFS
    {
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println( "Start updating " + type );
  });
  
  ArduinoOTA.onEnd( []() 
  {
    Serial.println( "\nEnd" );
  });
  
  ArduinoOTA.onProgress( [](unsigned int progress, unsigned int total ) 
  {
    Serial.printf( "Progress: %u%%\r", (progress / (total / 100)) );
  });
  
  ArduinoOTA.onError( []( ota_error_t error ) 
  {
    Serial.printf( "Error[%u]: ", error );
    if( error == OTA_AUTH_ERROR) { Serial.println( "Auth Failed" ); }
    else if(error == OTA_BEGIN_ERROR) { Serial.println("Begin Failed"); }
    else if(error == OTA_CONNECT_ERROR) { Serial.println("Connect Failed"); }
    else if(error == OTA_RECEIVE_ERROR) { Serial.println("Receive Failed"); }
    else if(error == OTA_END_ERROR) { Serial.println("End Failed"); }
  });
  ArduinoOTA.begin();
}

int getLevelForColor( String whichPostiion, String color )
{
  
  if( color == "red" )
  {
    if( whichPostiion == "second" )
    {
      return currentRSecond;
    }
    else
    {
      return currentRFirst;
    } 
  }
  else if( color == "green" )
  {
    if( whichPostiion == "second" )
    {
      return currentGSecond;
    }
    else
    {
      return currentGFirst;
    }
  }
  else if( color == "blue" )
  {
    if( whichPostiion == "second" )
    {
      return currentBSecond;
    }
    else
    {
      return currentBFirst;
    }
  }
  else if( color == "white" )
  {
    if( whichPostiion == "second" )
    {
      return currentWSecond;
    }
    else
    {
      return currentWFirst;
    }
  }
  else
  {
    return 0;
  }
}

float getRatioForColor( String whichPosition, String color )
{
  int  currentValue = getLevelForColor( whichPosition, color );
  return (float) ((float)currentValue / (float)1024);
}

// A tiny API server for a farm master node
void nodeServer()
{
   WiFiClient client = server.available();
   if( client )
   {
      String requestString; 
      boolean currentLineIsBlank = true;
      while( client.connected() )
      {
        if( client.available() )
        {
          char c = client.read();
          requestString += c;

          // request is complete, parse it
          if( c == '\n' && currentLineIsBlank )
          {
              htmlHeader( client );

              String requestPosition = "first";
              String requestColor = "all";
              int requestLevel = 1024;

              if( requestString.indexOf( "?action=id" ) > 0 )
              {
                byte mac[6];
                WiFi.macAddress( mac );
                // Using client.write multiple times caused scrolling output. Building a string causes instant output on completion, which is preferable
//                String output = nodeType;
//                output += "|";
                String output = String( mac[0], HEX );
                output +=  ":" ;
                output += String( mac[1], HEX );
                output +=  ":" ;
                output += String( mac[2], HEX );
                output +=  ":" ;
                output += String( mac[3], HEX );
                output +=  ":" ;
                output += String( mac[4], HEX );
                output +=  ":" ;
                output += String( mac[5], HEX );
                output += "|";
                output += WiFi.localIP();
//                output += "|";
//                output += nodeVersion;
                client.println( output );
                return; // We don't want to go below this if the request was for "id"
              }
              
              // parse the incoming request and do the work
              // p = "position" = first|second
              // c = "color" = red|green|blue|white|all
              // l ="level" = 0|25|50|75|100
              if( requestString.indexOf( "?p=second" ) > 0 || requestString.indexOf( "&p=second" ) > 0 )
              {
                requestPosition = "second";
              }
              if( requestString.indexOf( "?c=r" ) > 0 || requestString.indexOf( "&c=r" ) > 0 )
              {
                requestColor = "red";
              }
              if( requestString.indexOf( "?c=g" ) > 0 || requestString.indexOf( "&c=g" ) > 0 )
              {
                requestColor = "green";
              }
              if( requestString.indexOf( "?c=b" ) > 0 || requestString.indexOf( "&c=b" ) > 0 )
              {
                requestColor = "blue";
              }
              if( requestString.indexOf( "?c=w" ) > 0 || requestString.indexOf( "&c=w" ) > 0 )
              {
                requestColor = "white";
              }
              
              if( requestString.indexOf( "?l=0" ) > 0 || requestString.indexOf( "&l=0" ) > 0 )
              {
                requestLevel = 0;
              }
              if( requestString.indexOf( "?l=25" ) > 0 || requestString.indexOf( "&l=25" ) > 0 )
              {
                requestLevel = 256;
              }
              if( requestString.indexOf( "?l=50" ) > 0 || requestString.indexOf( "&l=50" ) > 0 )
              {
                requestLevel = 512;
              }
              if( requestString.indexOf( "?l=75" ) > 0 || requestString.indexOf( "&l=75" ) > 0 )
              {
                requestLevel = 768;
              }


              if( requestColor == "all"  )
              {
                if( requestLevel == 100 )
                {
                  turnOn( requestPosition );
                }
                else if( requestLevel == 0 )
                {
                  turnOff( requestPosition );
                }
                else
                {
                  setColorToLevel( requestPosition, "red", requestLevel );
                  setColorToLevel( requestPosition, "green", requestLevel );
                  setColorToLevel( requestPosition, "blue", requestLevel );
                  setColorToLevel( requestPosition, "white", requestLevel );
                }
              }
              else
              {
                setColorToLevel( requestPosition, requestColor, requestLevel );
              }

              client.println( getValues() );
              
              htmlFooter( client );
              break;
          } // if( c == '\n' && currentLineIsBlank )
          if( c == '\n' ) 
          {
            currentLineIsBlank = true;
          } 
          else if( c != '\r' ) 
          {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
          
        } // if( client.available() )
      } // while( client.connected() )

    // give the web browser time to receive the data
    delay( 1 );

    // close the connection:
    client.stop();
   } // if( client )
} // void nodeServer()

String getValues()
{
  String output = "<p>First Red value: ";
                output += getLevelForColor( "first", "red" );
                output += "<br>";
                output += "First Red ratio: ";
                output += getRatioForColor( "first", "red" );
                output += "&nbsp;</p>";

                output += "<p>First Green value: ";
                output += getLevelForColor( "first", "green" );
                output += "<br>";
                output += "First Green ratio: ";
                output += getRatioForColor( "first", "green" );
                output += "&nbsp;</p>";

                output += "<p>First Blue value: ";
                output += getLevelForColor( "first", "blue" );
                output += "<br>";
                output += "First Blue ratio: ";
                output += getRatioForColor( "first", "blue" );
                output += "&nbsp;</p>";

                output += "<p>First White value: ";
                output += getLevelForColor( "first", "white" );
                output += "<br>";
                output += "First White ratio: ";
                output += getRatioForColor( "first", "white" );
                output += "&nbsp;</p>";

                output += "<p>&nbsp;</p><p>Second Red value: ";
                output += getLevelForColor( "second", "red" );
                output += "<br>";
                output += "Second Red ratio: ";
                output += getRatioForColor( "second", "red" );
                output += "&nbsp;</p>";

                output += "<p>Second Green value: ";
                output += getLevelForColor( "second", "green" );
                output += "<br>";
                output += "Second Green ratio: ";
                output += getRatioForColor( "second", "green" );
                output += "&nbsp;</p>";

                output += "<p>Secondirst Blue value: ";
                output += getLevelForColor( "second", "blue" );
                output += "<br>";
                output += "Second Blue ratio: ";
                output += getRatioForColor( "second", "blue" );
                output += "&nbsp;</p>";

                output += "<p>Second White value: ";
                output += getLevelForColor( "second", "white" );
                output += "<br>";
                output += "Second White ratio: ";
                output += getRatioForColor( "second", "white" );
                output += "&nbsp;</p>";

  return output;
}

