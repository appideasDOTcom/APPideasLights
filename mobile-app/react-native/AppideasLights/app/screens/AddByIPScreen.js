import React, {Component} from 'react';
import {Button, Text, View, TextInput} from 'react-native';
import {StackActions, NavigationActions} from 'react-navigation';

import Realm from 'realm';
import {Schema_Controller, Schema_Light} from '../schema/SchemaObjects';

export class AddByIPScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Add by IP',
	    headerBackTitle: 'Back',
	  };
	
	saveInput( text )
	{
		var maxPosition = 0;
		
		Realm.open( { schema: [Schema_Controller, Schema_Light] } )
		.then(
			realm =>
			{
				let savedControllers = realm.objects( 'Controller' ).sorted( 'position', true );
				currentController = savedControllers[0];
				
				if( currentController != null )
				{
					maxPosition = currentController.position;
				}
				nextPosition = maxPosition + 1;
				
				realm.write(
 				() =>
 				{
 					realm.create( 'Controller',
 					{
 						position: nextPosition,
 						ipAddr: text,
 						niceName: text,
 					});
 				});
				
			}
		).catch(
			function( e )
			{
				console.log( e );
			}
		);
		
		
		const resetNavStack = StackActions.reset({
		    index: 0,
		    actions: [
		      NavigationActions.navigate({ routeName: 'Home'})
		    ] })
		this.props.navigation.dispatch( resetNavStack );
	}
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
	        <Text>What is the address?{"\n\n"}</Text>
	        <TextInput
	        	style={{height: 40, borderColor: 'gray', borderWidth: 1, width: '80%', backgroundColor: '#fff' }}
	        	onSubmitEditing = { (event) => this.saveInput( event.nativeEvent.text ) }
	        />
	      </View>
	    );
	  }
	}