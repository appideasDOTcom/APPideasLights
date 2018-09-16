import React, {Component} from 'react';
import {Button, Text, View} from 'react-native';

export class DetailsScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Details',
	    headerBackTitle: 'Back'
	  };
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center' }}>
	        <Text>Details Screen</Text>
	        <Button
	          title="Go to Home"
	          onPress={() => this.props.navigation.navigate('Home')}
	        />
	        <Button
	          title="Go back"
	          onPress={() => this.props.navigation.goBack()}
	        />
	      </View>
	    );
	  }
	}