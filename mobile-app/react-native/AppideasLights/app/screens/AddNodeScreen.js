import React, {Component} from 'react';
import {Button, Text, View} from 'react-native';

export class AddNodeScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Add lights',
	    headerBackTitle: ' ',
	  };
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#D3E3F1' }}>
	        <Text>Add a WiFi LED controller...{"\n\n"}</Text>
	        <Button
	          title="By IP address"
	          onPress={() => this.props.navigation.navigate('Home')}
	        />
	        <Text>{"\n"}</Text>
	        <Button
	          title="Scan network"
	          onPress={() => this.props.navigation.goBack()}
	        />
	      </View>
	    );
	  }
	}