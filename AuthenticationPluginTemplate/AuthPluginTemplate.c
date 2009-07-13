/*
 *	AuthPluginTemplate
 *
 *	Version 1 ($Revision$)
 * 
 *	Created by Karl Kuehn on 7/9/09.
 *	Copyright 2009 Karl Kuehn. All rights reserved.
 *
 *	This project is licensed under the Apache 2.0 License
 *
 *	This is a sample AuthorizationPlugin meant to help people writting loginwindow plugins.
 *		It should provide for all of the needs of a basic non-GUI plugin.
 *		At a minimum you should be doing something usefull in the MechanismInvoke method.
 *
 *	Notes:
 *		- The debug build has a preprocessor macro set to define "DEBUG" to cause more verbose logging
 *		- Only the Security framework is included, if you wish to use others you will need to add them
 *		- The latest version of this project can be found at http://code.google.com/p/loginwindowtools
 *
 *	The following documents were referenced durring the creation of this code:
 *		- http://developer.apple.com/documentation/Security/Reference/AuthorizationPluginRef/Reference/reference.html
 *		- http://developer.apple.com/technotes/tn2008/tn2228.html
 *		- http://developer.apple.com/samplecode/NullAuthPlugin/index.html
 *
 *	To the author(s) best efforts this code is free of bugs, but no garentee or warentee is made or implied
 *
 *	Version history: (a more complete history can be found in the SVN commit comments)
 *		1.0 - rev 2 - July 11, 2009 - intial import of the project
 *
*/

#import <Security/Security.h>
#import <asl.h>

#include <Security/AuthorizationPlugin.h>

#include "AuthPluginTemplate.h"

//============================
#pragma mark # Data strucutres
//============================

struct pluginStructure {
	const AuthorizationCallbacks *	callbacks;
};
typedef struct pluginStructure pluginStructure;


struct mechStructure {
	char *							mechName;	// name of the mechanism provided to MechanismCreate
	const AuthorizationCallbacks *	callbacks;	// access to the callbacks given to AuthorizationPluginCreate
	AuthorizationEngineRef			authEngine;	// authoriation engine provided to MechanismCreate
};
typedef struct mechStructure mechStructure;

static AuthorizationPluginInterface dispatchTable = {
    kAuthorizationPluginInterfaceVersion,		// version of the plugin system, this is deinfed at compile time
    &PluginDestroy,								// final method to release plugin resources
    &MechanismCreate,							// create an instance (user has hit return at the loginwindow)
    &MechanismInvoke,							// system is asking us to validate a login session
    &MechanismDeactivate,						// start ending the mechanism
    &MechanismDestroy							// login window is going away, clean up the mechanism
};


//===========================
#pragma mark # Plugin methods
//===========================

extern OSStatus AuthorizationPluginCreate (const AuthorizationCallbacks * callbacks, AuthorizationPluginRef * pluginRef, const AuthorizationPluginInterface ** pluginInterface) {		
	asl_warn("Plugin starting");
	
	#pragma mark setup pluginRef
	*pluginRef = (AuthorizationPluginRef *)malloc(sizeof(pluginStructure));
	if (pluginRef == NULL) {
		asl_error("Unable to malloc space for pluginRef structure");
		return errAuthorizationInternal;
	}
	((pluginStructure *) *pluginRef)->callbacks = callbacks;	// grab the callbacks handle
	
	#pragma mark designate the AuthorizationPluginInterface
	*pluginInterface = &dispatchTable; // declared in the header file
		
	asl_warn("Plugin started");
	return errAuthorizationSuccess;
}

static OSStatus PluginDestroy (AuthorizationPluginRef pluginRef) {
	/*
	 
	 Note: It appears that the loginwindow fails to call this method, and this does not appear to be specific to this code,
	 as it also affects the NullAuthPlugin sample code from Apple. A bug has been filed with Apple on this (rdar://7052473).
	 
	 For most plugins this probably does not matter as the loginwindow (and associated processes) exists when this would be
	 called taking any malloc'ed memory with it. The only issues would be with out-of-process resources (LDAP connections, etc).
	 
	*/
	
	asl_warn("Plugin finishing");
	
	#pragma mark cleanup pluginRef
	free(pluginRef);
	
	asl_warn("Plugin finished");
	return errAuthorizationSuccess;
}


//==============================
#pragma mark # Mechanism methods
//==============================

static OSStatus MechanismCreate (AuthorizationPluginRef pluginRef, AuthorizationEngineRef authEngine, AuthorizationMechanismId mechanismId, AuthorizationMechanismRef * mechRef) {
	asl_warn("Mechanism %s setting up", mechanismId);
	
	#pragma mark setup mechRef for this mechanism
	*mechRef = (AuthorizationMechanismRef *)malloc(sizeof(mechStructure));
	if (mechRef == NULL) {
		asl_error("Unable to malloc space for mechRef structure");
		return errAuthorizationInternal;
	}
	((mechStructure *) *mechRef)->callbacks = ((pluginStructure *) pluginRef)->callbacks; // copy the location of the callbacks to the mechRef
	((mechStructure *) *mechRef)->authEngine = authEngine;
	((mechStructure *) *mechRef)->mechName = malloc(strlen(mechanismId) + 1); // need space for the terminating NULL
	strncpy(((mechStructure *) *mechRef)->mechName, mechanismId, strlen(mechanismId) + 1);
	
	asl_warn("Mechanism %s set up", ((mechStructure *) *mechRef)->mechName);
	return errAuthorizationSuccess;
}

static OSStatus MechanismInvoke(AuthorizationMechanismRef mechRef) {
	asl_warn("Mechanism %s invoking", ((mechStructure *) mechRef)->mechName);
	
	// TODO implimentation code here

	#pragma mark accept the login
	if (errAuthorizationSuccess != ((mechStructure *) mechRef)->callbacks->SetResult(((mechStructure *) mechRef)->authEngine, kAuthorizationResultAllow)) {
		asl_error("Error setting the authorization result in mechanism %s", ((mechStructure *) mechRef)->mechName);
		return errAuthorizationInternal;
	}
	
	asl_warn("Mechanism %s done invoking", ((mechStructure *) mechRef)->mechName);
	return errAuthorizationSuccess;
}

static OSStatus MechanismDeactivate(AuthorizationMechanismRef mechRef) {
	asl_warn("Mechanism %s deactivating", ((mechStructure *) mechRef)->mechName);
	
	#pragma mark mark as deactivated
	OSStatus result = ((mechStructure *) mechRef)->callbacks->DidDeactivate(((mechStructure *) mechRef)->authEngine);
	
	asl_warn("Mechanism %s deactivated", ((mechStructure *) mechRef)->mechName);
	return result;
}

static OSStatus MechanismDestroy(AuthorizationMechanismRef mechRef) {
	asl_warn("Mechanism %s finsishing", ((mechStructure *) mechRef)->mechName);
	
	// copy the name so we can free the real copy
	char * mechName = malloc(strlen(((mechStructure *) mechRef)->mechName) + 1);
	strncpy(mechName, ((mechStructure *) mechRef)->mechName, strlen(((mechStructure *) mechRef)->mechName) + 1);
	
	#pragma mark cleanup mechRef
	free(((mechStructure *) mechRef)->mechName);
	free(mechRef);
	
	asl_warn("Mechanism %s finsihed", mechName);
	free(mechName);
	return errAuthorizationSuccess;
}

