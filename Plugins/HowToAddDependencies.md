1. Open XXX/XXX.uplugin, add requirements in the following format:
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

2. Open XXX/Source/XXX/XXX.Build.cs, find PublicDependencyModuleNames.AddRange/PrivateDependencyModuleNames.AddRange, add requirements in the following format:
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

3. Rebuild the whole project so that the plugin(module) would have Binaries&Intermediate folders