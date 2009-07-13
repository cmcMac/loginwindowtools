/*
 *	AklogAuthPlugin.h
 *	AklogAuthPlugin
 *
 *	Created by Karl Kuehn on 7/11/09 based on the AuthPluginTemplate from http://code.google.com/p/loginwindowtools
 *	Copyright 2009 Karl Kuehn. All rights reserved.
 *
 *	This project is licensed under the Apache 2.0 License
 *
*/

extern OSStatus AuthorizationPluginCreate(const AuthorizationCallbacks * callbacks, AuthorizationPluginRef * authRef, const AuthorizationPluginInterface ** pluginInterface);

static OSStatus MechanismCreate(AuthorizationPluginRef authRef, AuthorizationEngineRef authEngine, AuthorizationMechanismId mechanismId, AuthorizationMechanismRef * mechRef);

static OSStatus MechanismInvoke(AuthorizationMechanismRef mechRef);

static OSStatus MechanismDeactivate(AuthorizationMechanismRef mechRef);

static OSStatus MechanismDestroy(AuthorizationMechanismRef mechRef);

static OSStatus PluginDestroy(AuthorizationPluginRef authRef);


// setup ASL functions

#ifndef ASL_KEY_FACILITY
#define ASL_KEY_FACILITY "Facility"
#endif

#ifdef DEBUG
#define ASL_LOG_LEVEL	ASL_LEVEL_DEBUG
#else
#define ASL_LOG_LEVEL	ASL_LEVEL_WARNING
#endif

#define asl_level_log(level, format, ...) if (level <= ASL_LOG_LEVEL) { asl_log(NULL, NULL, level, format, ##__VA_ARGS__); };

#define asl_emerg(format, ...)	asl_level_log(ASL_LEVEL_EMERG, format, ##__VA_ARGS__)
#define asl_alert(format, ...)	asl_level_log(ASL_LEVEL_ALERT, format, ##__VA_ARGS__)
#define asl_crit(format, ...)	asl_level_log(ASL_LEVEL_CRIT, format, ##__VA_ARGS__)
#define asl_error(format, ...)	asl_level_log(ASL_LEVEL_ERR, format, ##__VA_ARGS__)
#define asl_warn(format, ...)	asl_level_log(ASL_LEVEL_WARNING, format, ##__VA_ARGS__)
#define asl_notice(format, ...)	asl_level_log(ASL_LEVEL_NOTICE, format, ##__VA_ARGS__)
#define asl_info(format, ...)	asl_level_log(ASL_LEVEL_INFO, format, ##__VA_ARGS__)
#define asl_debug(format, ...)	asl_level_log(ASL_LEVEL_DEBUG, format, ##__VA_ARGS__)
