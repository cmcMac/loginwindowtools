A loginwindow plugin (AuthorizationPlugin) to run `aklog` early in the login process to avoid problems with AFS home directories.

Note: this is not about Apple's AFP system, but rather with [Andrew File System](http://en.wikipedia.org/wiki/Andrew_File_System) setups.

Note: the current version of this plugin has never been tested in a production environment. The techniques it uses have, but this code has not.

The problem that it is meant to solve is that `aklog` needs to be run before the home directory is accessible. This can be a problem in some cases because the MCXCompositor that is used in WorkgroupManagement runs as part of the loginwindow process, well before the traditional method of using loginhooks can run. In addition to MCXCompositor running slowly (and contributing to logins taking a minute-and-a-half), a number of early-running processes (such as the dock) can run before the AFS process has had enough time to setup its connection and so those processes default to internal settings, and will even overwrite the users settings when the home directory is available.

The solution is to run `aklog` after the user has their Kerberos ticket (you have to set the loginwindow to get them a ticket), but before MCXCompositor is run. This plugin will do exactly this, the setup and notes are included in both the binary and source distributions.

The [binary version](http://loginwindowtools.googlecode.com/files/AklogAuthPlugin.dmg) of this plugin can be found in the [downloads section](http://code.google.com/p/loginwindowtools/downloads/list).