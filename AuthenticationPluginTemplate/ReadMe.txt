AuthoriszationPlugin Template
=====

Sample code to make creating AuthorizationPlugins to run at the loginwindow on MacOS X easier. This code is a fully working example, but does not actually do anything other than log events (when compiled in debug mode). It is released as open-source under the Apache 2 license as part of the LoginwindowTools project: http://code.google.com/p/loginwindowtools

It was written as example code for an article hosted at: http://wranglingmacs.blogspot.com/2009/07/writing-loginwindow-plugins.html, but it is hoped that it will have have a life that extends beyond the article. In addition to that article the Apple documentation and sample code are good resources:

http://developer.apple.com/technotes/tn2008/tn2228.html
http://developer.apple.com/documentation/Security/Reference/AuthorizationPluginRef/Reference/reference.html
http://developer.apple.com/documentation/Security/Reference/authorization_ref/Reference/reference.html
http://developer.apple.com/samplecode/NullAuthPlugin/index.html

Installing
-----

To use your plugin you need to do two things: put the bundle in the right place, and get the proper entry into the authorization policy database. For simplicities' sake I will just cover how to do this by editing /etc/authoization file directly. This works just fine in 10.4 and 10.5, but Apple can change this at any point so for real solutions you should use either /usr/bin/security or the Authoization Services C API.

Open the /etc/authorization file in your editor of choice (this file requires admin rights to write) and find the section at "rights->system.login.console" the default version should look like this:

    <key>system.login.console</key>
    <dict>
        <key>class</key>
        <string>evaluate-mechanisms</string>
        <key>comment</key>
        <string>Login mechanism based rule.  Not for general use, yet.</string>
        <key>mechanisms</key>
        <array>
            <string>builtin:smartcard-sniffer,privileged</string>
            <string>loginwindow:login</string>
            <string>builtin:reset-password,privileged</string>
            <string>builtin:auto-login,privileged</string>
            <string>builtin:authenticate,privileged</string>
            <string>loginwindow:success</string>
            <string>HomeDirMechanism:login,privileged</string>
            <string>HomeDirMechanism:status</string>
            <string>MCXMechanism:login</string>
            <string>loginwindow:done</string>
        </array>
    </dict>
    
To add your plugin to the loginwindow you add another line to the "mechanisms" array. The format should be: <plugin-name>:<mechanism-name>,privileged. Note that the "privileged" at the end is only used for non-GUI mechanisms that need root access. For many plugins the entry should probably go somewhere between the "loginwindow:success" entry and the "MCXMechanism" entry. If you need access to the home directory it should go the end of that list.

The sample plugin does not care about the mechanism name (you still need to provide one), so you can provide any string you would like.

Changing the plugin name
-----

When creating your own plugins you will probably want to change the name of the bundle created, and change the bundle identifier to reflect your organization's name. To change the bundle name:

1.  Open the project in XCode and open the "Targets" group in the "Groups & Files" pane to show the bundle's entry.

2.  Click on the "AuthPluginTemplate" item to highlight it, then from the "File" menu select "Get Info". This should bring up a window.

3.  In the 'Target "AuthPluginTemplate" Info' window make sure that "Configuration" is set to "All Configurations" and "Show" is "All settings" then type "Product Name" in the search box. This should reduce the options to just the one setting. If you double click on "AuthPlugin" a box where can change it to anything you would like will come up. You can close the window.

To change the bundle id:

1.  Open the "AuthPluginTemplate" group in "Groups & Files", then open the "Resources" folder inside that.

2.  Click on the "Info.plist" item and it should show the contents in the editing pane. Change the first section of the "Bundle identifier" section to match your organization, leaving the ".${PRODUCT_NAME:rfc1034Identifier}" at the end.

Once you have finished either (or both) of these you should probably select "Clean all targets" from the "Build" menu to get rid of old files, and then re-build your project to get the new version.
