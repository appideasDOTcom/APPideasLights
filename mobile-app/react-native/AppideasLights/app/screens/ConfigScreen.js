import React, {Component} from 'react';
import {Button, Text, View, TextInput, ScrollView, TouchableOpacity, Alert} from 'react-native';
import {StackActions, NavigationActions} from 'react-navigation';

import Realm from 'realm';
import {Schema_Controller, Schema_Light} from '../schema/SchemaObjects';

var incomingData = new Array();
nav = "";

var saveIP = "";
var saveName = "";

/**
 * Screen for configuring a controller
 * 
 * @author costmo
 * @since 20180825
 */
export class ConfigScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Configuration',
	    headerBackTitle: 'Back',
	  };
	
	/**
	 * Class constructor
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 * @param object	props	Application properties
	 */
	constructor( props )
	{
		super( props );
		incomingData = this.props.navigation.getParam( "itemData", new Array() );
		nav = props.navigation;
		
		this.state = { 
				dataPosition: incomingData.position, 
				dataIP: incomingData.ipAddr, 
				dataNiceName: incomingData.niceName,
				dataStrip1Id: incomingData.strip1Id,
				dataStrip2Id: incomingData.strip2Id,
		};
	}
	
	/**
	 * Give an "Are you sure?" dialog before deleting
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 */
	alertForDelete()
	{
		Alert.alert(
			'Please Confirm',
			'Are you sure you want to remove ' + this.state.dataNiceName + '?',
			[
				{ text: 'NO', onPress: () => { /* Just dismiss */ } },
				{ text: 'YES', onPress: () => this.deleteDataItem() },
			],
			{
				cancelable: false
			}
		);
	}
	
	/**
	 * Performs the deletion of the current controller
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 */
	deleteDataItem()
	{
		const classState = this.state;
		
		Realm.open( { schema: [Schema_Controller, Schema_Light] } )
		.then(
			function( realm )
			{
				// remove connectged lights
				var filter = 'id = ' + classState.dataStrip1Id + ' OR id = ' + classState.dataStrip2Id;
				let removeLights = realm.objects( 'Light' ).filtered( 'id = ' + classState.dataStrip1Id + ' OR id = ' + classState.dataStrip2Id  );
				realm.beginTransaction();
				for( let i = 0; i < removeLights.length; i++ )
				{
					let currentLight = removeLights[i];
					realm.delete( currentLight );
				}
				realm.commitTransaction();
				
				// delete the controller
				var positionString = "position = " + classState.dataPosition;
				let removeControllers = realm.objects( 'Controller' ).filtered( positionString );
				realm.beginTransaction();
				for( let i = 0; i < removeControllers.length; i++ )
				{
					let currentController = removeControllers[i];
					realm.delete( currentController );
				}
				realm.commitTransaction();
				
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
	
	/**
	 * Updates the state values that represent the controller's data values
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 * @param string	which	Which field to save input for
	 * @param string	value	The input value to save
	 */
	updateDataItem( which, value )
	{
		if( "ip" == which )
		{
			this.state.dataIP = value;
		}
		else if( "name" == which )
		{
			this.state.dataNiceName = value;
		}
	}
	
	/**
	 * Save user input to the database
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 */
	saveInput()
	{
		if( this.state.dataIP.length < 1 || this.state.dataNiceName.length < 1 )
		{
			alert( "Please fill in all fields." );
		}
		else
		{
			const classState = this.state;
			
			Realm.open( { schema: [Schema_Controller, Schema_Light] } )
			.then(
				function( realm )
				{
					var positionString = "position = " + classState.dataPosition;
					let saveControllers = realm.objects( 'Controller' ).filtered( positionString );
					
					for( let i = 0; i < saveControllers.length; i++ )
					{
						let currentController = saveControllers[i];
						
						realm.write(
						() =>
						{
							currentController.niceName = classState.dataNiceName;
							currentController.ipAddr = classState.dataIP;
						});
					}
					
					
				}
			).catch(
				function( e )
				{
					console.log( e );
				}
			);
			
			
			this.props.navigation.goBack();
		}
		
	}
	
	/**
	 * Render the screen
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 */
	  render() {
	    return (
	      <View style={{ flex: 1, backgroundColor: '#D3E3F1', paddingTop: 50, paddingLeft: 20 }}>
	      <ScrollView contentContainerStyle={{flexGrow: 1 }}>
		      
		      		<Text>Name{"\n"}</Text>
		      		<TextInput
		      			style={{height: 40, borderColor: 'gray', borderWidth: 1, width: '90%', backgroundColor: '#fff' }}
		      			defaultValue = { incomingData.niceName }
			      		onChangeText=
			      		{ 
			      			(text) => 
				      		{
				      			this.setState( {text} );
				      			this.updateDataItem( "name", text );
				      		}
		      			}
		      		/>
		      
		      <Text>{"\n\n"}</Text>
		      
	      		<Text>IP address{"\n"}</Text>
	      		<TextInput
	      			style={{height: 40, borderColor: 'gray', borderWidth: 1, width: '90%', backgroundColor: '#fff' }}
	      			defaultValue = { incomingData.ipAddr }
		      		onChangeText=
		      		{ 
	      				(text) => 
			      		{
			      			this.setState( {text} );
			      			this.updateDataItem( "ip", text );
			      		}
		  			}
	      		/>
	      		
	      		<Text>{"\n\n"}</Text>
	      		<Button style={{ flex: 1, alignItems: 'center' }}
		          title="SAVE"
		          onPress={ () => { this.saveInput(); } }
		        />
		       
		        <Text>{"\n\n\n\n"}</Text>
		        
	      		<Button style={{ flex: 1, alignItems: 'center', backgroundColor: '#f00' }}
		          title="DELETE"
		          onPress={ () => { this.alertForDelete(); } }
		        />
		       
		        <Text>{"\n\n\n\n"}</Text>
		        
	     
	      </ScrollView>
	      </View>
	    );
	  }
	}