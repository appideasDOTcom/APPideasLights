This is code that was written to accompany a custom-built WiFi controller for 5050SMD LED light strips. The Instructable that shows you how to build the hardware is here:
https://www.instructables.com/id/Easier-WiFi-LED-Light-Strip-Controller/
The APPideas article that covers start-to-finish instructions is available here:
https://appideas.com/the-appideas-wifi-controller-for-led-light-strips/

The Instructables article and the writing at APPideas are nearly identical, but are organized differently. The APPideas write-up is more likely to be kept up-to-date.

An Instructables article that covers version 9 (an older version) of the hardware is here:
https://www.instructables.com/id/WiFi-LED-Light-Strip-Controller/

The arduino directory contains code to be uploaded to an ESP8266-12E development board. Directions for loading the Arduino code are available at the links above. If you built hardware version 9 or earlier (anything prior to the "Easier" WiFi controller article), you will need to checkout the Arduino code from the alt/v9Hardware branch. Alternatively, you can manually change the Arduino code to adjust the pin assignments for that version of the PCB.

The resources directory contains the license file, files for CNC routers and 3D printers, and Fritzing, Eagle and Fusion 360 documents. V10 folders contain current files. V9 and previous, as well as Fritzing files are for legacy versions of the hardware.

The mobile-app directory has a React Native project, which produced native code for both Android and iOS.

## Mobile app installation
**Install the current public version from an app store**
The easiest way to install the app is through your device's app store. Here are the links:
Android: https://play.google.com/store/apps/details?id=com.appideas.appideaslights
iOS: https://itunes.apple.com/us/app/appideas-lights/id1440833432?ls=1&mt=8

**Install the latest development version from source code**
This will only work if you have React Native setup for development. Instructions are available here: https://facebook.github.io/react-native/docs/getting-started.html
Once React Native is setup for development, open a terminal and run these commands
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

The feature/patternLoops branch contains two more endpoints that have no access in the app (yet), but if you want to check out that branch and upload the Arduino code to your ESP board, you may also use these endpoints:
```
/multiset - Set the values of all LEDS (red, green, blue and white) with a single network request
/pattern - Run a pattern, such as strobe or color changes on a timer
```

See issue #12 (Programmable patterns) and issue #16 (Save colors).

## Misc

I have written a crap-ton of mobile apps, but this is my first in React Native. It is functional, but not written with best practices. A majority of the items on the Issues list are related to that fact. If you are an experienced React Native developer and wouldn't mind offering a few hours to refactor this code before the codebase grows to be large, that would be awesome. Issue #1 is the core of it all.

Check out part 1 of this Instructable series. It's about making custom PCBs on a CNC router:

https://www.instructables.com/id/Custom-PCBs-on-a-CNC-Router/

If you have a CNC router this will come in handy for making a custom WiFi constroller.

## License

This software is available under a GPLv3 license. A copy of the license is in resources/gpl/
