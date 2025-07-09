0. Open Editor, nevigate to `Plugins` in Content Drawer

1. Enter the `Public` folder of the plugin (XXX C++ Classes) you want to add subsystem to, right click and choose `New C++ Class`

2. Choose `All Classes` and type the class name (e.g. UUserWidget) in the lineEdit below, click on the result you want and click `Next`

3. Choose the target module in the dropdown of the comboBox on the left, edit the name and path if you want and click `Create Class`

4. `Tools` -> `Refresh Visual Studio Code Project`, close the editor

5. Open XXX/Source/XXX/XXX.Build.cs, find PublicDependencyModuleNames.AddRange, add UMG requirements in the following format:
```
{
    {
        ...;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                ...,
				"UMG",
				"Slate",
				"SlateCore",
			}
		);
	}
}
```

6. Rebuild the whole project to update the plugin(module)'s Binaries&Intermediate folders