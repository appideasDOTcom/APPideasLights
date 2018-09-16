import React, {Component} from 'react';
import {Button, Platform, StyleSheet, Text, View} from 'react-native';
import { NetworkInfo } from 'react-native-network-info';

export class HomeScreen extends React.Component {
	
	constructor( props )
	{
		super( props );
	}
	
	static navigationOptions = {
	    title: 'APPideas Lights',
	    headerBackTitle: ' '
	  };
	
	
	
	  render() 
	  {
		  const { navigate } = this.props.navigation;
		  
		  // Check to see what network we're on
		  NetworkInfo.getIPAddress( 
		  function( ip )
		  { 
			  if( ip.startsWith( "192.168.4." ) )
			  {
				  navigate( 'Connect' );
			  }
			  else
			  {
				  navigate( 'Nodes' );
			  }
			  console.log( ip ); 
		  });
		  
		  
		    return (
			      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
			        <Text>Getting things started...</Text>
			      </View>
		    );
	  } // render()
	
};

