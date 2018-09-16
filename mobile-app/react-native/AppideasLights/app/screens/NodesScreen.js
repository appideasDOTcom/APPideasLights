import React, {Component} from 'react';
import {Button, Text, View} from 'react-native';

nav = "";

export class NodesScreen extends React.Component {

	constructor( props ) 
	{
    super( props );
		this.state = {};
		nav = props.navigation;
	}
	
	static navigationOptions = {
	    title: 'APPideas Lights',
	    headerLeft: null,
	    headerRight: (
	            <Button
								onPress={ () => { nav.navigate( 'AddNode' ); } }
		            title="+"
		            color="#000"
		        />
	        ),
	  };
	
	  render() {
	    return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
	        <Text>You have not setup any WiFi LED controllers.{"\n\n"}Click the "+" button at the top to get started.</Text>
	      </View>
	    );
	  }
	}