import React, {Component} from 'react';
import {Button, Platform, StyleSheet, Text, View} from 'react-native';
import { NetworkInfo } from 'react-native-network-info';
import store from 'react-native-simple-store';

nodeArray = [];

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
		  
		for( var i = 0; i < 16; i++ )
		{
			var controllerName = "controller_" + i;

				store.get( controllerName )
				.then(
					(res) =>
						{
							try
							{
									var data = { "ipAddr": res.ipAddr, "name": res.name };
									nodeArray.push( data );
							}
							catch( e ){} // catch
						} // (res) function
				); // then

				
		} // for 
		  
		  // Check to see what network we're on
		  NetworkInfo.getIPAddress( 
		  function( ip )
		  { 
			  // If we're on the ESP network, go to the connection screen
			  if( ip.startsWith( "192.168.4." ) )
			  {
				  navigate( 'Connect' );
			  }
			  else
			  {
				  navigate( 'Nodes', 
					{
						itemData: nodeArray
					} );
			  }
		  });
		  
		  
		    return (
			      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
			        <Text>Getting things started...</Text>
			      </View>
		    );
	  } // render()
	
};

