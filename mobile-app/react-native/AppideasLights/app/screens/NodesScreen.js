import React, {Component} from 'react';
import {Button, Text, View} from 'react-native';
import GridView from 'react-native-super-grid';

import Realm from 'realm';
import {Schema_Controller, Schema_Light} from '../schema/SchemaObjects';

nav = "";
handledItemCount = false;
savedItemCount = 0;
nodeArray = [];

/**
 * Screen for displaying all controllers
 * 
 * @author costmo
 * @since 20180825
 */
export class NodesScreen extends React.Component {

	/**
	 * Class constructor
	 * 
	 * @author costmo
	 * @since 20180825
	 * @param object	props	Application properties
	 */
	constructor( props ) 
	{
		super( props );
		this.updateSavedItemCount();
		this.state = { blankView: true };
		nodeArray = this.props.navigation.getParam( 'itemData', new Array() );
		nav = props.navigation;
	}
	
	/**
	 * Set an internal state when there is data to show
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 */
	changeView()
	{
	     this.setState(
	     {
	    	 blankView: false
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
		
		/**
		 * Gets the count of saved items and set the appropriate member
		 * 
		 * @author costmo
		 * @since 20180825
		 * @return void
		 */
		updateSavedItemCount()
		{
			savedItemCount = 0;

			Realm.open( { schema: [Schema_Controller, Schema_Light] } )
			.then(
				function( realm )
				{
					let savedControllers = realm.objects( 'Controller' );
					savedItemCount = savedControllers.length;
				}
			).catch(
				function( e )
				{
					console.log( e );
				}
			);

			if( savedItemCount > 0 )
			{
				this.setState(
				{
					blankView: false
				});
			}

			this.finalizeSavedItemCount();
		} // updateSavedItemCount()()

		/**
		 * Change the view state when all promises have been fulfilled and we have the item count
		 * 
		 * @author costmo
		 * @since 20180825
		 * @return void
		 */
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

			if( nodeArray.length > 0 )
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

	/**
	 * Render class for when there are controllers to show
	 * 
	 * @author costmo
	 * @since 20180825
	 * @return void
	 */
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
								title= { item.niceName }
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