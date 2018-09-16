import React, {Component} from 'react';
import {Button, Text, View, TextInput, ScrollView, TouchableOpacity, Alert} from 'react-native';
import store from 'react-native-simple-store';
import {StackActions, NavigationActions} from 'react-navigation';

var incomingData = new Array();
nav = "";

var saveIP = "";
var saveName = "";

export class ConfigScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Configuration',
	    headerBackTitle: 'Back',
	  };
	
	constructor( props )
	{
		super( props );
		incomingData = this.props.navigation.getParam( "itemData", new Array() );
		nav = props.navigation;
		
		this.state = { dataIP: incomingData.ipAddr, dataName: incomingData.name, dataNiceName: incomingData.niceName };
	}
	
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
	
	deleteDataItem()
	{
		store.delete( incomingData.name );
		const resetNavStack = StackActions.reset({
		    index: 0,
		    actions: [
		      NavigationActions.navigate({ routeName: 'Home'})
		    ] })
		this.props.navigation.dispatch( resetNavStack );
	}
	
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
	
	saveInput()
	{
		if( this.state.dataIP.length < 1 || this.state.dataNiceName.length < 1 )
		{
			alert( "Please fill in all fields." );
		}
		else
		{
			store.update( this.state.dataName, 
					{ 
						ipAddr: this.state.dataIP,
						name: this.state.dataName,
						niceName: this.state.dataNiceName,
					}
			);
			this.props.navigation.goBack();
		}
		
	}
	
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