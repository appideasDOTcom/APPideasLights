import React, {Component} from 'react';
import {Button, Text, View, Switch, Slider, ScrollView} from 'react-native';

var ipAddr = "";
nav = "";
var incomingData = new Array();

export class ControlsScreen extends React.Component {
	
	constructor( props )
	{
		super( props );
		this.state = { lightsOn1: true, lightsOn2: true, 
				rValue1: 100, gValue1: 100, bValue1: 100, wValue1: 100,
				rValue2: 100, gValue2: 100, bValue2: 100, wValue2: 100
				};
		
		incomingData = this.props.navigation.getParam( "itemData", new Array() );
		ipAddrr = incomingData.ipAddr;
		console.log( "Incoming IP: " + ipAddrr );
		nav = props.navigation;
	}
	
	setAllSliders( whichPosition, value )
	{
		if( "first" == whichPosition )
		{
			this.setState( { rValue1: value } );
			this.setState( { gValue1: value } );
			this.setState( { bValue1: value } );
			this.setState( { wValue1: value } );
		}
		else
		{
			this.setState( { rValue2: value } );
			this.setState( { gValue2: value } );
			this.setState( { bValue2: value } );
			this.setState( { wValue2: value } );
		}
	}
	
	static navigationOptions = {
	    title: 'Controls',
	    headerBackTitle: 'Back',
	    headerRight: (
	            <Button
					onPress={ () => { nav.navigate( 'Config', 
							{
								itemData: incomingData
							} ); } }
		            title="Config"
		            color="#000"
		        />
	        ),
	  };
	
	handleToggle( whichSet )
	{
		var args = "p=" + whichSet;
		args += "&c=all";
		
		if( "first" == whichSet )
		{
			if( this.state.lightsOn1 )
			{
				args += "&l=0";
				this.setState( { lightsOn1: false } );
				this.setAllSliders( whichSet, 0 );
			}
			else
			{
				args += "&l=100";
				this.setState( { lightsOn1: true } );
				this.setAllSliders( whichSet, 100 );
			}
		}
		else
		{
			if( this.state.lightsOn2 )
			{
				args += "&l=0";
				this.setState( { lightsOn2: false } );
				this.setAllSliders( whichSet, 0 );
			}
			else
			{
				args += "&l=100";
				this.setState( { lightsOn2: true } );
				this.setAllSliders( whichSet, 100 );
			}
		}
		var response = this.sendCommand( "control", args );
	}
	
	handleSlide( section, color, value )
	{
		var args = "p=" + section;
		args += "&c=" + color;
		args += "&l=" + value;
		var response = this.sendCommand( "control", args );
		
		if( "red" == color )
		{
			if( "first" == section )
			{
				this.setState( { rValue1: value } );
			}
			else
			{
				this.setState( { rValue2: value } );
			}
		}
		else if( "green" == color )
		{
			if( "first" == section )
			{
				this.setState( { gValue1: value } );
			}
			else
			{
				this.setState( { gValue2: value } );
			}
		}
		else if( "blue" == color )
		{
			if( "first" == section )
			{
				this.setState( { bValue1: value } );
			}
			else
			{
				this.setState( { bValue2: value } );
			}
		}
		else if( "white" == color )
		{
			if( "first" == section )
			{
				this.setState( { wValue1: value } );
			}
			else
			{
				this.setState( { wValue2: value } );
			}
		}
	}
	
	  render() {
		  
	    return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 20 }}>
	      <ScrollView contentContainerStyle={{flexGrow: 1 }}>
	        <Text>First set of lights{"\n"}</Text>
	        <Switch
	        	onValueChange = { () => { this.handleToggle( "first" ); } }
	        	value = { this.state.lightsOn1 }
	        />
	        
	        <Text>{"\n"}</Text>
	        <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 8 }}>R </Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "red", value ) } }
		          value={ this.state.rValue1 }
		        />
		     </View>
		      
		     <Text>{"\n"}</Text>
		     <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 8 }}>G </Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "green", value ) } }
		          value={ this.state.gValue1 }
		        />
		     </View>
		     
		     <Text>{"\n"}</Text>
		     <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 8 }}>B </Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "blue", value ) } }
		          value={ this.state.bValue1 }
		        />
		     </View>
		      
		     <Text>{"\n"}</Text>
		     <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 8 }}>W </Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "white", value ) } }
		          value={ this.state.wValue1 }
		        />
		     </View>
		      
		      <Text>{"\n\n\n\n"}</Text>
		      
		      
		      <Text>Second set of lights{"\n"}</Text>
		        <Switch
		        	onValueChange = { () => { this.handleToggle( "second" ); } }
		        	value = { this.state.lightsOn2 }
		        />
		        
		        <Text>{"\n"}</Text>
		        <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 8 }}>R </Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "red", value ) } }
			          value={ this.state.rValue2 }
			        />
			     </View>
			      
			     <Text>{"\n"}</Text>
			     <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 8 }}>G </Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "green", value ) } }
			          value={ this.state.gValue2 }
			        />
			     </View>
			     
			     <Text>{"\n"}</Text>
			     <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 8 }}>B </Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "blue", value ) } }
			          value={ this.state.bValue2 }
			        />
			     </View>
			      
			     <Text>{"\n"}</Text>
			     <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 8 }}>W </Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "white", value ) } }
			          value={ this.state.wValue2 }
			        />
			     </View>
			     <Text>{"\n\n"}</Text>
		  </ScrollView>
	      </View>
	    );
	  }
	  
	  sendCommand( path, args )
	  {
		  var url = "http://" + ipAddr + ":5050/" + path + "?" + args;
		  return fetch( url )
		    .then(
		    		(response) => 
		    		{
		    			var jsonString = JSON.stringify( response );
		    			var json = JSON.parse( jsonString );
		    			var bodyJsonString = JSON.stringify( json._bodyInit );
		    			var bodyJson = JSON.parse( json._bodyInit );

		    			return( bodyJson );
		    			response.json();
		    		})
		    .then((responseJson) => {
		      return responseJson;
		    })
		    .catch((error) => {
		      console.error(error);
		    });
	  }
	  
}