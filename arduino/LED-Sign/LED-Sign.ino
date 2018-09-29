#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

/**
 * A sketch to control two strings of RGBW LED lights from one ESP8266.
 * 
 * This creates its own wireless access point with an SSID beginning with "appideas-"
 *   that you need to connect to for configuring this device to connect to your network
 *   
 * For complete assembly and usage information, visit this Instructable:
 * https://www.instructables.com/id/Wifi-Led-Light-Strip-Controller/
 * 
 * @author costmo
 * @since  20180902
 */

// Setup a wireless access point so the user can be prompted to connect to a network
String  ssidPrefix = "appideas-";
int serverPort = 5050;
String ssid = "";
String softIP  = "";
bool wifiConnected = false;
const int MAX_WIFI_RETRIES = 60; // The maximum number of times we'll try connecting to WiFi before reverting to AP mode

// Setup NTP parameters for syncing with network time
const int TIMEZONE_OFFSET = 3600; // Number of seconds in an hour
int inputTimezoneOffset = 0; // Offset from UTC - will be input by the user
const char* ntpServerName = "time.nist.gov"; // NTP server to use
const int checkInterval = 60 * 30; // how often (in seconds) to query the NTP server for updates (30 minutes)
// NTP parameters that probably shouldn't be changed
unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

// For reference (pin mapping for ESP8266-12E):
// D0/GPIO16      = 16;
// D1/GPIO5       = 5;
// D2/GPIO4       = 0;
// D3/GPIO0       = 4;

// D4/GPIO2       = 2;
// D5/GPIO14      = 14;
// D6/GPIO12      = 13;
// D7/GPIO13      = 12;


// Pin mapping for the first string of lights. Pins D0 - D3
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

int status = WL_IDLE_STATUS;
IPAddress ip;

ESP8266WebServer server( serverPort );

// Status variables
// Temporary holding for current values
int currentR = 0;
int currentG = 0;
int currentB = 0;
int currentW = 0;

// holding for current values on first and second string of lights
int currentRFirst = 0;
int currentGFirst = 0;
int currentBFirst = 0;
int currentWFirst = 0;

int currentRSecond = 0;
int currentGSecond = 0;
int currentBSecond = 0;
int currentWSecond = 0;

// Global time variables
String fullTime = String( "" );
String humanTime = String( "" );
int intTime = 0;
int currentHour = 0;
int currentMinute = 0;
int currentSecond = 0;
int currentMeridiem = 0;

// Values saved to EEPROM for persistent storage of settings
struct StoredSettings
  {
    char ssid[128];
    char pass[128];
    int offset = 0;
  } romSettings;

/**
 * Run on device power-on
 * 
 * @author costmo
 * @since  20180902
 */
void setup() 
{
  Serial.begin( 57600 );
  delay( 100 );

  // Prepare the pins to fire
  pinMode( WHITE_LED_FIRST, OUTPUT );
  pinMode( BLUE_LED_FIRST, OUTPUT );
  pinMode( RED_LED_FIRST, OUTPUT );
  pinMode( GREEN_LED_FIRST, OUTPUT );

  pinMode( WHITE_LED_SECOND, OUTPUT );
  pinMode( BLUE_LED_SECOND, OUTPUT );
  pinMode( RED_LED_SECOND, OUTPUT );
  pinMode( GREEN_LED_SECOND, OUTPUT );

  // uncomment the line below to erase your stored network credentials when the device powers on
  //wipeSettings();
  
  // Check to see if we have stored network credentials already
  if( !haveNetworkCredentials() )
  {
    // Start the local access point
    wipeSettings();
    startSoftAP();
  }
  else // Attempt to connect to the local network based on stored credentials
  {
    StoredSettings deviceSettings = getStoredSettings();
    connectToWifi( String( deviceSettings.ssid ), String( deviceSettings.pass ) );
  }
  // Start the web server and get ready to handle incoming requests
  startWebServer();

  // turn all the lights on when the ESP first powers on
  turnOn( "first" );
  turnOn( "second" );
}

/**
 * Whether or not we have stored network credentials. Will return false of the stored SSID is "appideas-changeme"
 * 
 * @return bool
 * @author costmo
 * @since  20180929
 */
bool haveNetworkCredentials()
{
  StoredSettings deviceSettings = getStoredSettings();

  // < 2 to catch empty string
  if( deviceSettings.ssid == NULL || 
      (sizeof( deviceSettings.ssid ) < 2 || 
      String( deviceSettings.ssid ).equals( "appideas-changeme" ) )
  {
    return false; 
  }

  return true;
}

/**
 * Turn all lights to max brightness
 * 
 * @return void
 * @author costmo
 * @since  20180902
 * @param  String   whichPosition     first|second
 */
void turnOn( String whichPosition ) 
{ 
  setColorToLevel( whichPosition, "all", maxBrightness );
}

/**
 * Turn all lights off
 * 
 * @return void
 * @author costmo
 * @since  20180902
 * @param  String   whichPosition     first|second
 */
void turnOff( String whichPosition ) 
{
  setColorToLevel( whichPosition, "all", offBrightness );
}

/**
 * Set a color at a position to a specific level between 0 and 1024
 * 
 * @return void
 * @author costmo
 * @since  20180902
 * @param  String   whichPosition     first|second
 * @param  String   color             red|green|blue|white|all
 * @param  intng    level             0 - 1024
 */
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

/**
 * Run while the sketch is active
 * 
 * @return void
 * @author costmo
 * @since  20180902
 */
void loop() 
{
  // Listen for API requests
  server.handleClient();
}

/**
 * Handler for requests to "/"
 * 
 * This should only provide the interface for logging into a network or show a "you are already connected" message
 * 
 * @return void
 * @author costmo
 * @since  20180913
 */
void handleRoot()
{
  if( !wifiConnected )
  {
    Serial.println( "Connecting" );
    server.send( 200, "text/html", connectionHtml() );
  }
  else
  {
    Serial.println( "Already connected" );
    server.send( 200, "text/html", connectedHtml() );
  }
  delay( 100 );
}

/**
 * Handler for requests to "/rollcall"
 * 
 * This is here so an app can easily scan a network and find nodes
 * 
 * @return void
 * @author costmo
 * @since  20180913
 */
void handleRollCall()
{
  String sendValue = "";
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["status"] = "present";
  root["mac"] = WiFi.macAddress();
  root.printTo( sendValue );
  
  server.send( 200, "text/html", sendValue );
}

/**
 * Sends a blank JSON string
 * 
 * This is used for network interactions in which the requestor is expecting a JSON response, but for the 
 *   sake of efficiency, we do not want to add extra processing to generate something meaningful
 * 
 * @return void
 * @author costmo
 * @since  20180913
 */
void sendBlank()
{
  String sendValue = "";
  StaticJsonBuffer<32> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root[""] = "";
  root.printTo( sendValue );
  
  server.send( 200, "text/html", sendValue );
}

/**
 * Handler for requests to "/connect" Connects to the local wifi network, establishes an NTP connection and synchronizes the time
 * 
 * connectToWifi() and getTime() do all the actual work
 * 
 * @see connectToWifi
 * @return void
 * @author costmo
 * @since  20180913
 */
void handleConnect()
{
  String hardSSID = server.arg( "ssid" );
  String hardPassword = server.arg( "password" );
  int tmpInputTimezoneOffset = server.arg( "timezone" ).toInt();

  if( tmpInputTimezoneOffset != 0 )
  {
    inputTimezoneOffset = tmpInputTimezoneOffset;
  }

  saveSettings( hardSSID, hardPassword, inputTimezoneOffset );

  if( !wifiConnected )
  {
    ssid = hardSSID;
    connectToWifi( ssid, hardPassword );
  }

  String newIp = WiFi.localIP().toString();
  server.send( 200, "text/html", redirect( newIp ) ); // Redirecting really doesn't work since the device will already be on a different network. We can still try, tough.

  // Get and show the time
  Serial.println( "Starting UDP" );
  udp.begin( localPort );
  fullTime = getTime();
  timeToVars();
  Serial.println( "Current time: " + humanTime );
  
  server.send( 200, "text/html", connectedHtml() );
  Serial.println( "Connected to WiFi" );
  delay( 100 );
}

/**
 * Start the local access point
 * 
 * @return void
 * @author costmo
 * @since  20180913
 */
void startSoftAP()
{
  String mac = WiFi.macAddress();
  String macPartOne = mac.substring( 12, 14 );
  String macPartTwo = mac.substring( 15 );
  ssid = ssidPrefix + macPartOne + macPartTwo; // "appideas-" followed by the last 4 characters of the device MAC address
  
  WiFi.softAP( ssid.c_str() );
  softIP = WiFi.softAPIP().toString();
 
  Serial.println( getSoftAPStatus() );
}

/**
 * Handler for requests to "/network-status"
 * 
 * Sends a JSON encoded string to the web browser containing the device's network status
 * 
 * @return void
 * @author costmo
 * @since  20180913
 */
void handleNetworkStatus()
{
   server.send( 200, "text/html", getNetworkStatus() );
}

/**
 * Handler for requests to "/status"
 * 
 * Sends a JSON encoded string to the web browser containing the status device's lights
 * 
 * @return void
 * @author costmo
 * @since  20180913
 */
void handleStatus()
{
   server.send( 200, "text/html", getStatus() );
}

/**
 * Define the endpoints for a tiny API server for receiving and handling requests and start the web server
 * 
 * @return void
 * @author costmo
 * @since  20180902
 */
void startWebServer()
{
  server.on( "/", handleRoot );
  server.on( "/connect", handleConnect );
  server.on( "/network-status", handleNetworkStatus );
  server.on( "/status", handleStatus );
  server.on( "/control", handleControl );
  server.on( "/rollcall", handleRollCall );
  server.begin();
}

/**
 * Connect to a wifi network and print the status
 * 
 * @return void
 * @author costmo
 * @since  20180902
 */
void connectToWifi( String wifiSSID, String wifiPassword )
{
  int attemptCount = 0;
  
  Serial.print( "Connecting to " );
  Serial.println( wifiSSID );
  
  WiFi.mode( WIFI_STA );
  WiFi.begin( wifiSSID.c_str(), wifiPassword.c_str() );

  while( WiFi.status() != WL_CONNECTED )
  {
      delay( 500 ); // delay one-half second between retries
      Serial.print( "." );

      // Reset EEPROM values and go into AP mode if the connection hasn't been established for a period of time. Default: 30 seconds
      if( attemptCount > MAX_WIFI_RETRIES )
      {
        wipeSettings();
        startSoftAP();
        return;
      }
      attemptCount++;
  }
  Serial.println( "." );
  Serial.println( "Connected" );
  wifiConnected = true;
  printWifiStatus();
}

/**
 * Print the current IP address
 * 
 * @return void
 * @author costmo
 * @since  20180902
 */
void printWifiStatus() 
{
  // print your WiFi IP address:
  ip = WiFi.localIP();
  Serial.print( "IP Address: " );
  Serial.println( ip );
}
 
/**
 * Gets the current level (0 - 1024) for the given color and position
 * 
 * @return int
 * @author costmo
 * @since  20180902
 * @param  String   whichPosition     first|second
 * @param  String   color             red|green|blue|white|all
 */
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

// 
/**
 * Gets the current ratio (0.0 - 1.0) for the given color and position
 * 
 * @return float
 * @author costmo
 * @since  20180902
 * @param  String   whichPosition     first|second
 * @param  String   color             red|green|blue|white|all
 */
float getRatioForColor( String whichPosition, String color )
{
  int  currentValue = getLevelForColor( whichPosition, color );
  return (float) ((float)currentValue / (float)1024);
}

/**
 * Handles API requests to control lights
 * 
 * @return void
 * @author costmo
 * @since  20180902
 */
void handleControl()
{
  // parse the incoming request and do the work
  // p = "position" = first|second
  // c = "color" = red|green|blue|white|all
  // l ="level" = 0-100

  String lightPosition = server.arg( "p" );
  String lightColor = server.arg( "c" );
  int lightLevel = server.arg( "l" ).toInt();

  String requestPosition = "first";
  String requestColor = lightColor;
  float ratio = ((float)lightLevel / (float)100);
  int requestLevel = ceil( ratio * maxBrightness );

  if( lightPosition.equals( "second" ) )
  {
    requestPosition = "second";
  }

  if( requestColor == "all"  )
  {
    setColorToLevel( requestPosition, "red", requestLevel );
    setColorToLevel( requestPosition, "green", requestLevel );
    setColorToLevel( requestPosition, "blue", requestLevel );
    setColorToLevel( requestPosition, "white", requestLevel );
  }
  else
  {
    setColorToLevel( requestPosition, requestColor, requestLevel );
  }
  delay( 50 ); // give the web server a small amount of time to buffer and send
  // server.send( 200, "text/html", getStatus() );
  sendBlank();
  server.send( 200, "text/html", "" ); // see if we can improve response times a little bit
}

/**
 * Get the current time from an NTP server
 * 
 * @return String
 * @author costmo
 * @since  20180902
 */
String getTime()
{
  
  //Serial.println( "Getting time" );
  String returnValue = String();

  //get a random server from the pool
  WiFi.hostByName( ntpServerName, timeServerIP );

  sendNTPpacket( timeServerIP ); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay( 2000 );
  
  int cb = udp.parsePacket();
  // loop until we have a good connection
  while( !cb ) 
  {
    //Serial.println( "Problem reading from NTP server. Retrying in 10 seconds..." );
    delay( 10000);
    //Serial.println( "Trying again..." );
    cb = udp.parsePacket();
  }

    // We've received a packet, read the data from it
    udp.read( packetBuffer, NTP_PACKET_SIZE ); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word( packetBuffer[40], packetBuffer[41] );
    unsigned long lowWord = word( packetBuffer[42], packetBuffer[43] );
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;

    epoch = epoch + (TIMEZONE_OFFSET * (inputTimezoneOffset + 1));

    int hour = ((epoch  % 86400L) / 3600);
    int minute = ((epoch  % 3600) / 60);
    int second = (epoch % 60);
    
    // make a 12 hour clock
    bool isPM = false;
    if( hour >= 12 )
    {
      isPM = true;
      if( hour > 12 )
      {
        hour = hour - 12;
      }
    }
    if( hour == 0 )
    {
      hour = 12;
    }
    
    returnValue = String( hour );
    returnValue = String( returnValue + " " );
    returnValue = String( returnValue + minute );
    returnValue = String( returnValue + " " );
    returnValue = String( returnValue + second );
    
    if( isPM )
    {
      returnValue = String( returnValue + " 1" );
    }
    else
    {
      returnValue = String( returnValue + " 0" );
    }

    humanTime = String( hour );
    humanTime = String( humanTime + ":" );
    humanTime = String( humanTime + minute );
    humanTime = String( humanTime + ":" );
    humanTime = String( humanTime + second );
    if( isPM )
    {
      humanTime = String( humanTime + " PM" );
    }
    else
    {
      humanTime = String( humanTime + " AM" );
    }

  return returnValue;
}

/**
 * Writes all time components to member variables for easier parsing and display.
 * 
 * This should only be run immediately following a call to getTime()
 * 
 * TODO: Make minutes and seconds < 10 zero-padded
 * 
 * @return void
 * @author costmo
 * @since  20180902
 */
void timeToVars()
{
  int separator1 = fullTime.indexOf( ' ' );
  int separator2 = fullTime.indexOf( ' ', (separator1 + 1) );
  int separator3 = fullTime.indexOf( ' ', (separator2 + 1) );

  String hourAsString = fullTime.substring( 0, separator1 );
  String minuteAsString = fullTime.substring( separator1, separator2 );
  String secondAsString = fullTime.substring( separator2, separator3 );
  String meridiemAsString = fullTime.substring( separator3 );


  currentHour = hourAsString.toInt();
  currentMinute = minuteAsString.toInt();
  int second = secondAsString.toInt();

  currentSecond = second;
  if( currentSecond >= 60 )
  {
    currentSecond = (currentSecond % 60);
    currentMinute += 1;
  }

  if( currentMinute >= 60 )
  {
    currentMinute = (currentMinute % 60);
    currentHour += 1;
  }

  if( currentHour >= 12 )
  {
    currentMeridiem = 1;
    currentHour = (currentHour % 12);
    if( currentHour == 0 )
    {
      currentHour = 11;
    }
    currentHour += 1;
  }
  else
  {
    currentMeridiem = 0;
  }

  if( currentHour < 12 && currentMeridiem > 0 )
  {
    currentHour = currentHour + 12;
  }

  String stringMinute = String( currentMinute );
  if( currentMinute < 10 )
  {
    stringMinute = "0" + stringMinute;
  }

  String timeString = String( currentHour );
  timeString = timeString + stringMinute;
  intTime = timeString.toInt();
}

 /**
 * Send an NTP request to the time server at the given address
 * 
 * @return long
 * @author costmo
 * @since  20180902
 * @param  IPAddress    address       A pointer to the IP address of the NTP server
 */
unsigned long sendNTPpacket( IPAddress& address )
{
  //Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

/**
 * Gets the network status and returns info as a JSON-encoded string
 * 
 * @return String
 * @author costmo
 * @since  20180913
 */
String getNetworkStatus()
{
  String returnValue = "";

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  if( wifiConnected )
  {
    root["soft"] = "disconnected";
    root["hard"] = "connected";
    root["ip"] = WiFi.localIP().toString();
  }
  else
  {
    root["soft"] = "connected";
    root["hard"] = "disconnected";
    root["ip"] = softIP;
  }
  root["ssid"] = ssid;

  root.printTo( returnValue );

  return returnValue;
}

/**
 * Gets the status of each color on each string of lights and returns info as a JSON-encoded string
 * 
 * @return String
 * @author costmo
 * @since  20180913
 */
String getStatus()
{
  String returnValue = "";

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  JsonArray& fv = root.createNestedArray( "firstvalues" );
  JsonArray& fr = root.createNestedArray( "firstratios" );
  JsonArray& sv = root.createNestedArray( "secondvalues" );
  JsonArray& sr = root.createNestedArray( "secondratios" );

  fv.add( getLevelForColor( "first", "red" ) );
  fv.add( getLevelForColor( "first", "green" ) );
  fv.add( getLevelForColor( "first", "blue" ) );
  fv.add( getLevelForColor( "first", "white" ) );

  fr.add( getRatioForColor( "first", "red" ) );
  fr.add( getRatioForColor( "first", "green" ) );
  fr.add( getRatioForColor( "first", "blue" ) );
  fr.add( getRatioForColor( "first", "white" ) );

  sv.add( getLevelForColor( "second", "red" ) );
  sv.add( getLevelForColor( "second", "green" ) );
  sv.add( getLevelForColor( "second", "blue" ) );
  sv.add( getLevelForColor( "second", "white" ) );

  sr.add( getRatioForColor( "second", "red" ) );
  sr.add( getRatioForColor( "second", "green" ) );
  sr.add( getRatioForColor( "second", "blue" ) );
  sr.add( getRatioForColor( "second", "white" ) );

  root.printTo( returnValue );

  return returnValue;
}

/**
 * Gets the SSIS and IP address of the local access point
 * 
 * @return String
 * @author costmo
 * @since  20180913
 */
String getSoftAPStatus()
{
  String returnValue = "\n";
  returnValue += "SSID: " + ssid + "\n";
  returnValue += "IP  : " + softIP + "\n";

  return returnValue;
}

/**
 * HTML form for connecting to a WiFi network
 * 
 * @return const String
 * @author costmo
 * @since  20180913
 */
const String connectionHtml()
{
  const String returnValue =
    "<!DOCTYPE HTML>"
    "<html>"
    "<head>"
    "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
    "<title>Connect to WiFi</title>"
    "<link href='https://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css'>"
    "<style>"
    "body { background-color: #D3E3F1; font-family: \"Open Sans\", sans-serif; Color: #000000; }"
    "#header { width: 100%; text-align: center; }"
    "input[type=text], input[type=password] { width: 50%; height: 30px; padding-left: 10px; }"
    "</style>"
    "</head>"
    "<body>"
    "<div id='header'><h3>Connect to your WiFi network</h3></div>"
    "<FORM action=\"/connect\" method=\"post\">"
    "<P>"
    "<p><INPUT type=\"text\" name=\"ssid\" placeholder=\"SSID\" autocomplete=\"off\" autocorrect=\"off\" autocapitalize=\"off\" spellcheck=\"false\"></p>"
    "<p><INPUT type=\"password\" name=\"password\" placeholder=\"Password\" autocomplete=\"off\" autocorrect=\"off\" autocapitalize=\"off\" spellcheck=\"false\"></p>"
    "<p><INPUT type=\"text\" name=\"timezone\" placeholder=\"Timezone Offset (e.g. -8)\" autocomplete=\"off\" autocorrect=\"off\" autocapitalize=\"off\" spellcheck=\"false\"></p><br>"
    "<p><INPUT type=\"submit\" value=\"CONNECT\"></p>"
    "</P>"
    "</FORM>"
    "</body>"
    "</html>";

  return returnValue;
}

/**
 * HTML to show when a user is already connected and is on a page that offers or processes connections
 * 
 * @return const String
 * @author costmo
 * @since  20180913
 */
const String connectedHtml()
{
  const String returnValue =
    "<!DOCTYPE HTML>"
    "<html>"
    "<head>"
    "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
    "<title>Connect to WiFi</title>"
    "<link href='https://fonts.googleapis.com/css?family=Open+Sans' rel='stylesheet' type='text/css'>"
    "<style>"
    "body { background-color: #D3E3F1; font-family: \"Open Sans\", sans-serif; Color: #000000; }"
    "#header { width: 100%; text-align: center; }"
    "</style>"
    "</head>"
    "<body>"
    "You are connected to WiFi"
    "</body>"
    "</html>";

  return returnValue;
}

/**
 * Attempt to redirect users after successful attempt to connect to a WiFi network
 * 
 * This doesn't currently work since the user is disconnected from the ESP prior to this being an option to be offered
 * 
 * @return String
 * @author costmo
 * @since  20180902
 */
String redirect( String newIP )
{
  String returnValue =
    "<!DOCTYPE HTML>"
    "<html>"
    "<head>"
    "<script type=\"text/javascript\">"
    " window.location = \"http://" + newIP + "/\""
    "</script>"
    "</head>"
    "<body>"
    "</body>"
    "</html>";

  return returnValue;
}

/**
 * Stores the input settings to EEPROM
 * 
 * @return void
 * @author costmo
 * @since  20180929
 */
void saveSettings( String ssid, String password, int offset )
{
  uint addr = 0;

  strcpy( romSettings.ssid, ssid.c_str() );
  strcpy( romSettings.pass, password.c_str() );
  romSettings.offset = offset;

  EEPROM.begin( 512 );
  EEPROM.put( addr, romSettings );
  EEPROM.end();
}

/**
 * Retrieves stored settings from EEPROM so that WiFi setup does not have to be repeated on every power cycle
 * 
 * @return struct
 * @author costmo
 * @since  20180929
 */
StoredSettings getStoredSettings()
{
  uint addr = 0;
  EEPROM.begin( 512 );
  EEPROM.get( addr, romSettings );
  EEPROM.end();
  return romSettings;
}

/**
 * Removes all the input settings from EEPROM
 * 
 * The ESP8266 EEPROM wasn't working the same as Arduino for blanking stored values, so we
 *   set a known "bad" value to make the network credentials be invalid
 * 
 * @return void
 * @author costmo
 * @since  20180929
 */
void wipeSettings()
{
  struct StoredSettings
  {
    char ssid[128];
    char pass[128];
    int offset = 0;
  } clearSettings;

  strcpy( clearSettings.ssid, "appideas-changeme" );
  strcpy( clearSettings.pass, "apideas-changeme" );

  uint addr = 0;
  EEPROM.begin( 512 );
  EEPROM.put( addr, clearSettings );
  EEPROM.end();
}
