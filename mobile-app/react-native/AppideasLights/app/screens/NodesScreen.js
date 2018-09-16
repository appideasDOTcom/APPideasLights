import React, {Component} from 'react';
import {Button, Text, View} from 'react-native';
import store from 'react-native-simple-store';
import GridView from 'react-native-super-grid';

nav = "";
handledItemCount = false;
savedItemCount = 0;
nodeArray = [];

export class NodesScreen extends React.Component {

	constructor( props ) 
	{
		super( props );
		this.updateSavedItemCount();
		this.state = { blankView: true };
		nodeArray = this.props.navigation.getParam( 'itemData', new Array() );
		nav = props.navigation;
	}
	
	changeView()
	{
	     this.setState(
	     {
	    	 blankView: !this.state.blankView
	     })
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
		
		updateSavedItemCount()
		{
			savedItemCount = 0;

			for( var i = 0; i < 16; i++ )
			{
				var controllerName = "controller_" + i;

				store.get( controllerName )
				.then(
					(res) =>
						{
							try
							{
									var incomingIP = res.ipAddr;

									if( incomingIP.length > 0 )
									{
										savedItemCount++;
									}
									if( i >= 15 )
									{
										this.finalizeSavedItemCount();
									}
							}
							catch( e )
							{
								if( i >= 15 )
								{
									this.finalizeSavedItemCount();
								}
							} // catch
						} // (res) function
				); // then
			} // for
		} // updateSavedItemCount()()

		makeGrid( itemCount )
		{
				
		}

		finalizeSavedItemCount()
		{
			if( !handledItemCount )
			{
				handledItemCount = true;

				if( savedItemCount > 0 )
				{
					this.changeView();
				}
			}
		}
	
	  render() {

			if( !this.state.blankView )
		  {
			  return <AvailableNodes changeView = { () => this.changeView() } />
			}
			
			// This will render while the stuff above gets its results, then will disappear if there are items to show
			return (
	      <View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50 }}>
	        <Text>You have not setup any WiFi LED controllers.{"\n\n"}Click the "+" button at the top to get started.</Text>
	      </View>
	    );
	  } // render()
}

export default class AvailableNodes extends Component
{

	constructor( props )
	{
		super( props );
	}

	render()
	{
		return(
			<View style={{ flex: 1, alignItems: 'center', backgroundColor: '#D3E3F1', paddingTop: 50, height: 75 }}>
				<GridView
					itemDimension = {130}
					items = {nodeArray}
					renderItem = {item => (
						<View style={{ flex: 1, alignItems: 'center', backgroundColor: '#fff', paddingTop: 20, paddingBottom: 20, borderColor: 'gray', borderWidth: 1 }}>
							<Button style={{ flex:1, assignSelf: 'stretch', width:'100%', height:'100%', backgroundColor: '#fff' }}
								title={item.ipAddr}
								onPress={ () => nav.navigate( 'Control', 
									{
										itemData: item
									} )  }
							/>
						</View>
					) }
				/>
			</View>
		);
	}
}