0. Open the editor, click `Content Drawer`, then nevigate to `Plugins`

1. Enter the folder with the name of the plugin (XXX Content) you want to add blueprint to, right click and choose `User Interface` -> `Widget Blueprint`

2. Select the widget blueprint, edit the name if you want

3. Open the widget blueprint, switch to Blueprint Designer Mode by clicking `Designer`

4. Design your layout (Quick start: Find the canvasPanel widget in `Palette` -> `Panel` -> `Canvas Panel`, drag it to the designer area)

5. Click `Compile`, then close the editor

6. Clean project files manually or run cleanProjectFiles.bat

7. Launch *.uproject file

8. `Tools` -> `Generate Visual Studio Code Project`

9. Open the widget blueprint, switch to Graph Editting Mode by clicking `Graph`

10. `Class Settings` -> `Details` -> `Class Options` -> `Parent Class`, select the parent class you want to reparent to