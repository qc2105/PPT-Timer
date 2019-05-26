function Component()
{
}

Component.prototype.createOperations = function() 
{
	component.createOperations();
	if ( installer.value("os") === "win" ) 
	{
                component.addOperation("CreateShortcut", "@TargetDir@/PPT-Timer.exe", "@StartMenuDir@/PPT-Timer.lnk" );
	}
}
