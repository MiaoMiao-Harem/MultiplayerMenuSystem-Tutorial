## How to create plugin

1. Open the editor, `Edit` -> `Plugins` -> `New Plugin`, choose `Blank` and input the plugin name below, then click `Create Plugin`

2. Click `Content Drawer`, `Settings` -> `Show Plugin Content`, then close the editor

3. Open Plugins/XXX/XXX.uplugin, add requirements in the following format:
```
{
	...,

    "Plugins": [
        {
            "Name": "%Module1%",
            "Enabled": %true/false%
        },
        {
            "Name": "%Module2%",
            "Enabled": %true/false%
        },
        ...
    ]
}
```

4. Open Plugins/XXX/Source/XXX/XXX.Build.cs, find PublicDependencyModuleNames.AddRange/PrivateDependencyModuleNames.AddRange, add requirements in the following format:
```
{
    {
        ...;

		%Public/Private%DependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "%Module1%",
                "%Module2%",
                ...
			}
		);
	}
}
```

6. Rebuild the whole project so that the plugin(module) would have Binaries&Intermediate folders


## How to add plugin

1. Nevigate to the directory where 'Plugins' folder is located, copy the folder

2. Enter the directory where target project files are located, paste the folder


## How to add subsystem class to plugin

0. Open the editor, click `Content Drawer`, then nevigate to 'C++ Classes'

1. Enter the folder with the name of the project, right click and choose `New C++ Class`

2. Choose `All Classes` and type `'UGameInstanceSubsystem'` in the lineEdit below, click on the result you want and click `Next`

3. Choose the target module in the dropdown of the comboBox on the left, edit the name and path if you want and click `Create Class`


## How to add menu class to plugin

0. Open the editor, click `Content Drawer`, then nevigate to 'Plugins'

1. Enter the `Public` folder of the plugin (XXX C++ Classes) you want to add subsystem to, right click and choose `New C++ Class`

2. Choose `All Classes` and type `'UUserWidget'` in the lineEdit below, click on the result you want and click `Next`

3. Choose the target module in the dropdown of the comboBox on the left, edit the name and path if you want and click `Create Class`

4. `Tools` -> `Refresh Visual Studio Code Project`, then close the editor

5. Open Plugins/XXX/Source/XXX/XXX.Build.cs, find `PublicDependencyModuleNames.AddRange`, add UMG requirements in the following format:
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