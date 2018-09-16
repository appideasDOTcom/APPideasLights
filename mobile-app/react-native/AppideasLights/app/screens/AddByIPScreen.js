import React, {Component} from 'react';
import {Button, Text, View, TextInput} from 'react-native';
import store from 'react-native-simple-store';
import {StackActions, NavigationActions} from 'react-navigation';

export class AddByIPScreen extends React.Component {
	
	static navigationOptions = {
	    title: 'Add by IP',
	    headerBackTitle: 'Back',
	  };
	
	saveInput( text )
	{
		store.update( 'controller_0', 
				{ 
					ipAddr: text,
					name: 'controller_0',
					niceName: text,
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