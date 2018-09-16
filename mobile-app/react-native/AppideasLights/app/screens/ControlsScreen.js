import React, {Component} from 'react';
import {Button, Text, View, Switch, Slider, ScrollView} from 'react-native';

var ipAddr = "";

export class ControlsScreen extends React.Component {
	
	constructor( props )
	{
		super( props );
		this.state = { lightsOn1: true, lightsOn2: true, rValue: 100, gValue: 100, bValue: 100, wValue: 100 };
		ipAddr = this.props.navigation.getParam( 'ipAddr', '0' );
	}
	
	static navigationOptions = {
	    title: 'Controls',
	    headerBackTitle: 'Back'
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
			}
			else
			{
				args += "&l=100";
				this.setState( { lightsOn1: true } );
			}
		}
		else
		{
			if( this.state.lightsOn2 )
			{
				args += "&l=0";
				this.setState( { lightsOn2: false } );
			}
			else
			{
				args += "&l=100";
				this.setState( { lightsOn2: true } );
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
			this.setState( { rValue: value } );
		}
		else if( "green" == color )
		{
			this.setState( { gValue: value } );
		}
		if( "blue" == color )
		{
			this.setState( { bValue: value } );
		}
		if( "white" == color )
		{
			this.setState( { wValue: value } );
		}
		
		alert( "Slide: " + args );
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
	          <Text style={{ paddingTop: 6 }}>R</Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "red", value ) } }
		          value={ this.state.rValue }
		        />
		     </View>
		      
		     <Text>{"\n"}</Text>
		     <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 6 }}>G</Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "green", value ) } }
		          value={ this.state.rValue }
		        />
		     </View>
		     
		     <Text>{"\n"}</Text>
		     <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 6 }}>B</Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "blue", value ) } }
		          value={ this.state.rValue }
		        />
		     </View>
		      
		     <Text>{"\n"}</Text>
		     <View style={{ flexDirection: 'row' }}>
	          <Text style={{ paddingTop: 6 }}>W</Text>
		      <Slider
		      	  style={{ width: '80%' }}
		          step={25}
		          minimumValue={0}
		          maximumValue={100}
		          onValueChange={ (value) => { this.handleSlide( "first", "white", value ) } }
		          value={ this.state.rValue }
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
		          <Text style={{ paddingTop: 6 }}>R</Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "red", value ) } }
			          value={ this.state.rValue }
			        />
			     </View>
			      
			     <Text>{"\n"}</Text>
			     <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 6 }}>G</Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "green", value ) } }
			          value={ this.state.rValue }
			        />
			     </View>
			     
			     <Text>{"\n"}</Text>
			     <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 6 }}>B</Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "blue", value ) } }
			          value={ this.state.rValue }
			        />
			     </View>
			      
			     <Text>{"\n"}</Text>
			     <View style={{ flexDirection: 'row' }}>
		          <Text style={{ paddingTop: 6 }}>W</Text>
			      <Slider
			      	  style={{ width: '80%' }}
			          step={25}
			          minimumValue={0}
			          maximumValue={100}
			          onValueChange={ (value) => { this.handleSlide( "second", "white", value ) } }
			          value={ this.state.rValue }
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
		    	//alert( "RETURN: " + JSON.stringify( responseJson ) );
		      return responseJson;
		    })
		    .catch((error) => {
		      console.error(error);
		    });
	  }
	  
}