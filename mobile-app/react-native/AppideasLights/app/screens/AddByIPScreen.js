import React, {Component} from 'react';
import {Button, Text, View, TextInput} from 'react-native';
import store from 'react-native-simple-store';

export class AddByIPScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Add by IP',
	    headerBackTitle: 'Back',
	  };
	
	saveInput( text )
	{
		store.update( 'controller_0', 
				{ ipAddr: text }
		);
		this.props.navigation.goBack();
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