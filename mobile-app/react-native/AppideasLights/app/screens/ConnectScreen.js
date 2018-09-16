import React, {Component} from 'react';
import {Button, Text, View, WebView} from 'react-native';

export class ConnectScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Connect',
	    headerBackTitle: ' '
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