AklogAuthPlugin
=====

If you are using AFS for network home directories, especially if your setup uses WorkgroupManager/MCX, you might find that the traditional method of running aklog from a loginhook does not work. The problem is that there are a few processes (MCXCompositor being the first of them) that need access to the home directory before the loginhook has a chance to run.

The solution offered by this code is to run aklog as a part of the loginwindow's authorization process by creating a custom AuthorizationPlugin. 'aklog' is run as the user who is logging in, and the users dock preferences are read out, to make sure that AFS has a chance to start caching the user's home folder.

The code is offered as open-source under the Apache 2.0 license. It is based off the AuthorizationPluginTemplate code, which is also part of the LoginwindowTools project:

http://code.google.com/p/loginwindowtools

All future versions of the code should be available through that page.


Installing
-----

Put the AklogAuthPlugin.bundle in /Library/Security/SecurityAgentPlugins, and then make sure that all files in the bundle are owner/group root/wheel (chown -R), and also make sure that they are not writeable by anyone but root (otherwise you have a huge security hole). 

For simple deployments you can add the following line to the "rights->system.login.console->mechanisms" section of the /etc/authorization file. Notably you will want to add this line before the "MCXMechanism:login" entry:

	AklogAuthPlugin:Aklog,privileged

Apple recommends that you not edit this file directly as the format might change, but rather use either /usr/bin/security or the Authorization Services C API.

You should already have editied the authorization system to get Kerberos tickets:

http://support.apple.com/kb/TA20987


Notes
-----

*	If you are compiling the plugin yourself, you should compile the "debug" configuration for testing, but deploy only the "development" version. The former logs more information to syslog, while the latter only logs errors.
