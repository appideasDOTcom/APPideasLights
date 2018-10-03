import Realm from 'realm';

export class Schema_Controller extends Realm.Object {}
Schema_Controller.schema = 
{
	name: 'Controller',
	properties:
	{
		position: 'int' ,
		ipAddr: 'string',
		niceName: { type: 'string' },
		strip1Id: { type: 'int', default: -1 },
		strip2Id: { type: 'int', default: -1 },
	},
		
};

export class Schema_Light extends Realm.Object {}
Schema_Light.schema = 
{
	name: 'Light',
	properties:
	{
		id: { type: 'int'  },
		name: { type: 'string' },
	},
		
};
