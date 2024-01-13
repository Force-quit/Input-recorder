function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/Input-recorder.exe", "@StartMenuDir@/Input-recorder.lnk",
            "workingDirectory=@TargetDir@");
    }
}