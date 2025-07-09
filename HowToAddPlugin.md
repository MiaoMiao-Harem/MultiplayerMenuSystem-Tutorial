1. Open the editor, `Edit` -> `Plugins` -> `New Plugin`, choose `Blank` and input the plugin name below, then click `Create Plugin`

2. Click `Content Drawer`, `Settings` -> `Show Plugin Content`, then close the editor

3. Nevigate to 'Plugins' folder

4. Open XXX/XXX.uplugin, add requirements in the following format:
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

5. Open XXX/Source/XXX/XXX.Build.cs, find PublicDependencyModuleNames.AddRange/PrivateDependencyModuleNames.AddRange, add requirements in the following format:
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