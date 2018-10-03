import React, {Component} from 'react';
import {Button, Text, View, TextInput} from 'react-native';

/**
 * Screen for adding a controller by scanning the local network. Not yet implemented.
 * 
 * @author costmo
 * @since 20180825
 */
export class AddByScanScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Add by scan',
	    headerBackTitle: 'Back',
	  };
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
	        <Text>This is not yet implemented.{"\n\n"}</Text>
	      </View>
	    );
	  }
	}