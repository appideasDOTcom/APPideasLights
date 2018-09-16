import React, {Component} from 'react';
import {Button, Text, View, Switch} from 'react-native';

export class ControlsScreen extends React.Component {
	
	constructor( props )
	{
		super( props );
		this.state = { lightsOn: true };
	}
	
	static navigationOptions = {
	    title: 'Controls',
	    headerBackTitle: 'Back'
	  };
	
	handleToggle()
	{
		if( this.state.lightsOn )
		{
			this.setState( { lightsOn: false } );
		}
		else
		{
			this.setState( { lightsOn: true } );
		}
	}
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 20 }}>
	        <Text>Power{"\n"}</Text>
	        <Switch
	        	onValueChange = { () => { this.handleToggle(); } }
	        	value = { this.state.lightsOn }
	        />
	      </View>
	    );
	  }
	}