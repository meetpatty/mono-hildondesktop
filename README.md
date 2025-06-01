Source code for packages `mono-hildondesktop`, which provides CLI bindings for libhildondesktop
and `hildon-desktop-mono-loader`, a Hildon Desktop CLI Plugin Loader.\
\
Currently supports Mono 2.0.1 and Maemo 4.1 (Diablo).

## SDK Compilation and Installation

The following assumes a working Scratchbox 1 Diablo SDK with a `DIABLO_X86` target
and the Mono devkit installed.\
\
NOTE that testing C# plugins on the `DIABLO_ARMEL` target is not supported as the
Mono runtime is provided as an i386 devkit and cannot be embedded in an arm process.\
\
From the host run:
```sh
sudo chmod ugo+w /scratchbox/devkits/mono
```
From the `DIABLO_X86` target run:
```sh
./autogen.sh
./configure --prefix=/scratchbox/devkits/mono
make
make install
```
## Example Hildon Desktop Home Area Plugin 
 
### TestPlugin.cs
```c#
using System;
using Gtk;
using LibHildonDesktop;

public class TestPlugin : HomeItem
{
    private Label label;

    public TestPlugin() : base()
    {
        label = new Label("Hello from C#!");
        this.Add(label);
        this.ShowAll();
    }
}

public static class HdPluginFactory
{
    public static HomeItem[] GetObjects()
    {
        try
        {
            TestPlugin plugin = new TestPlugin();
            return new HomeItem[] { plugin };
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex.Message);
        }

        return new HomeItem[] { };
    }
} 
```
### TestPlugin.desktop
```
[Desktop Entry]
Name=Test C# Plugin
Type=mono
X-Path=TestPlugin.dll
```
### To compile and install run 
```sh 
mcs TestPlugin.cs `pkg-config --libs libhildondesktop-sharp` -target:library
cp TestPlugin.dll `pkg-config --variable=hildondesktoplibdir hildon-desktop`
cp TestPlugin.desktop `pkg-config --variable=homedesktopentrydir osso-af-settings`
```


