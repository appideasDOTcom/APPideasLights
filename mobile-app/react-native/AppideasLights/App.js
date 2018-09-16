
import React, {Component} from 'react';
import {AppRegistry, Platform, StyleSheet, Text, View} from 'react-native';
import { createStackNavigator } from 'react-navigation';
import { HomeScreen } from './app/screens/HomeScreen.js';
import { DetailsScreen } from './app/screens/DetailsScreen.js';
import { ConnectScreen } from './app/screens/ConnectScreen.js';
import { NodesScreen } from './app/screens/NodesScreen.js';
import { AddNodeScreen } from './app/screens/AddNodeScreen.js';

const RootStack = 
	createStackNavigator
	(
		  {
		    Home: HomeScreen,
		    Details: DetailsScreen,
		    Connect: ConnectScreen,
		    Nodes: NodesScreen,
		    AddNode: AddNodeScreen,
		  },
		  {
		    initialRouteName: 'Home',
		  }
	);

AppRegistry.registerComponent( 'RootStack', () => RootStack );
	
type Props = {};
export default class App extends Component<Props> 
{
  render() 
  {
      return <RootStack />;
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});
