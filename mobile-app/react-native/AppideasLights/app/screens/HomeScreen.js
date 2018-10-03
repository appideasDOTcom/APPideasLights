import React, {Component} from 'react';
import {Button, Platform, StyleSheet, Text, View} from 'react-native';
import { NetworkInfo } from 'react-native-network-info';
import store from 'react-native-simple-store'; 

import Realm from 'realm';
import {Schema_Controller, Schema_Light} from '../schema/SchemaObjects';

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

		Realm.open( { schema: [Schema_Controller, Schema_Light] } )
		.then(
			function( realm )
			{
				let savedControllers = realm.objects( 'Controller' ).sorted( 'position' );
				for( let i = 0; i < savedControllers.length; i++ )
				{
					let currentController = savedControllers[i];
					var data = { 
							"position": currentController.position,
							"ipAddr": currentController.ipAddr, 
							"niceName": currentController.niceName,
							"strip1Id": currentController.strip1Id,
							"strip2Id": currentController.strip2Id,
						};
					nodeArray.push( data );
				}
			}
		).catch(
			function( e )
			{
				console.log( e );
			}
		);
		
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

