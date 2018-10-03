import React, {Component} from 'react';
import {Button, Text, View, WebView} from 'react-native';

/**
 * Screen to bring up the AP's web interface to get it logged in to the local network
 * 
 * @author costmo
 * @since 20180825
 * @return void
 * @param string	text	The input to save
 */
export class ConnectScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Connect',
	    headerBackTitle: 'Back',
	    	headerLeft: null,
	  };
	
	  render() {
	    return (
	      <View style={{ flex: 1, backgroundColor: '#D3E3F1' }}>
		      <WebView
		        source={{uri: 'http://192.168.4.1:5050'}}
		      />
	      </View>
	    );
	  }
	}