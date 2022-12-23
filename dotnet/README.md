## NOTE
- **Newer C# NordicID.NurApi.Net library is available at nuget.org**
- **Samples and docs: https://github.com/NordicID/nur_sample_csharp**

###### NurApi C# libraries

This folder NurApi C# libraries for different targets.
- Samples [NordicID/nur_sample_windows](https://github.com/NordicID/nur_sample_windows)
- Docs [docs/NurApi .NET Documentation.chm](docs/NurApi%20.NET%20Documentation.chm)

###### Folders
- anycpu
  - Platform target: Any CPU
- x64
  - Platform target: x64
- x86
  - Platform target: x86
- x86_ansi
  - Platform target: x86 with CharSet.Ansi in DllImports (use with linux)
- windowsce
  - CF20
    - To use with windows ce ARM compact framework 2.0
  - CF35
    - To use with windows ce ARM compact framework 3.5
    
    
###### Windows
NurApiDotNet.dll needs NURAPI.dll to be found in path.
- Copy needed NURAPI.dll from native libraries to your project


###### Linux
NurApi C# library can be used under linux via mono.
- Copy NurApiDotNet.dll from x86_ansi folder
- Create NurApiDotNet.dll.config (see below)
- Copy needed native NurApi library in same folder (libNurApix64, libNurApix86, libNurApiRaspi)

###### Linux x86, x64
For x86 and x64 platforms, create following file "NurApiDotNet.dll.config" in running directory
```
<configuration>
   <dllmap dll="NurApi.dll" target="libNurApix64.so" cpu="x86-64" />
   <dllmap dll="NurApi.dll" target="libNurApix86.so" cpu="x86" /> </configuration>
</configuration>
```

###### Linux Raspberry Pi
For raspberry pi platform, create following file "NurApiDotNet.dll.config" in running directory
```
<configuration>
   <dllmap dll="NurApi.dll" target="libNurApiRaspi.so"/>   
</configuration>
```
