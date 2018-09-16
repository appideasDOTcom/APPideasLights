import React, {Component} from 'react';
import {Button, Text, View, TextInput} from 'react-native';

var incomingData = new Array();
nav = "";

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
	}
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
	        <Text>This is not yet implemented.{"\n\n"}</Text>
	      </View>
	    );
	  }
	}