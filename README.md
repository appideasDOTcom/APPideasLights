This is code that was written to accompany a custom-built WiFi controller for 5050SMD LED light strips. The Instructable that shows you how to build the hardware is here:
https://www.instructables.com/id/Wifi-Led-Light-Strip-Controller/

The Arduino directory contains code to be uploaded to an ESP8266-12E development board. Directions for loading the Arduino code are available at the Instructable link above.

The mobile-app directory has a React Native project, which produced native code for both Android and iOS.

## Mobile app installation
**If you have an Android device**
The easiest way to install the app is through the Play Store. You can get it here:
https://play.google.com/store/apps/details?id=com.appideas.appideaslights

**If you have an iOS device**
I'm working to get approval for the app from Apple so that it will be available from the App Store. Until then, you will need to follow the "Install from source code" instructions below

**Install from source code**
This will only work if you have React Native setup for development. Instructions are available here: https://facebook.github.io/react-native/docs/getting-started.html
Open a terminal and run these commands
```
mkdir app
cd app
git clone https://github.com/appideasDOTcom/APPideasLights.git ./
cd mobile-app/react-native/AppideasLights
npm install
```
To install for iOS, plug your device into your computer and:
```
react-native run-ios
```
To install for Android, plug your device into your computer and:
```
react-native run-android
```
If compiling or installation fails, run the last command again. This is expected behavior for a first build in React Native

## Using the mobile app

If your controller has not been configured to connect to your network, go into your device's WiFi settings and connect to the access point that is created by the ESP. It will begin with "appideas-" After connecting to the access point, open the app. Alternatively, open a web browser and go to http://192.168.4.1:5050/ Supply your network credentials and click CONNECT. Connect your mobile device to your network again, then close and re-open the app. Tap "+" to add a controller by IP address. Once the controller is added, the main screen of the app will have a button showing its IP address. Tap the button to see the controls. To change the name of the controller or to update the IP address, tap the Config button.

## Controlling lights from a web browser

The ESP8266 controller will respond to http requests to the following endpoints:
```
/ - Displays either the screen that allows you to provide WiFi credentials or a notice that you are already connected
/connect - This only accepts POST data from the root node. Nothing useful will be gained by accessing it directly
/network-status - Get the device's network info in a JSON-encoded string
/status - Get the status of each set of lights in a JSON-encoded string
/rollcall - Verify that the device is responding in a known manner
/control - Control a set of lights. Possible GET parameters are: 
p: 'position' Accepts 'first' or 'second'
c: 'color' Accepts 'red' 'green' 'blue' 'white' or 'all'
l: 'level' Accepts any integer between 0 (off) and 100 (100% power)
```

Here's an example call to the controller:
```
http://10.0.42.241:5050/control?p=first&c=red&l=100
```

## Misc

I have written a crap-ton of mobile apps, but this is my first in React Native. It is functional, but not written with best practices. A majority of the items on the Issues list are related to that fact. If you are an experienced React Native developer and wouldn't mind offering a few hours to refactor this code before the codebase grows to be large, that would be awesome. Issue #1 is the core of it all.

If you're here before the Instructable is published, check out this Instructable about making custom PCBs on a CNC router:

https://www.instructables.com/id/Custom-PCBs-on-a-CNC-Router/

If you have a CNC router this will come in handy for making a custom WiFi constroller.
