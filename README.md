This is code that was written to accompany a custom-built WiFi controller for 5050SMD LED light strips. 

The Arduino directory contains code to be uploaded to an ESP8266-12E development board.

The mobile-app directory has a React Native project, which produced native code for both Android and iOS.

Installation instructions will be available at Instructables.com once the write-up is done (early November).

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

## Installation

**Clone the repository (everyone):**
```
mkdir app
cd app
git clone https://github.com/appideasDOTcom/APPideasLights.git ./
cd mobile-app/react-native/AppideasLights
```

**Install the app in a React Native development environment (if you have React Native development already running on your computer:**
```
npm install
react-native run-ios
```
or
```
npm install
react-native run-android
```
If compiling or installation fails, run the second command again. This is normal.

**Install the app for iOS:**


**Install the app for Android:**


If you use the app, you have no reason to know or remember the endpoints or their inputs - that's all handled by the app.

I have written a crap-ton of mobile apps, but this is my first in React Native. It is functional, but not written with best practices. A majority of the items on the Issues list are related to that fact. If you are an experienced React Native developer and wouldn't mind offering a few hours to refactor this code before the codebase grows to be large, that would be awesome. Issue #1 is the core of it all.

If you're here before the Instructable is published, check out this Instructable about making custom PCBs on a CNC router:

https://www.instructables.com/id/Custom-PCBs-on-a-CNC-Router/

If you have a CNC router this will come in handy for making a custom WiFi constroller.
