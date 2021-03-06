# Codename One Extensions

![GitHub repo size](https://img.shields.io/github/repo-size/ericlight/CN1Extensions?style=plastic)
![GitHub release](https://img.shields.io/github/v/release/ericlight/CN1Extensions?style=plastic)
![GitHub release date](https://img.shields.io/github/release-date/ericlight/CN1Extensions?style=plastic)
![GitHub last commit](https://img.shields.io/github/last-commit/ericlight/CN1Extensions?color=red&style=plastic)

### Overview :
Codename One Extensions is a desktop tool to manage Codename One libraries (CN1LIB) in your projects. 
You can use it to explore, download, install, uninstall a CN1LIB from any Codename One projects. It has more features than the Codename One official built-in extensions manager.
You can [download this app](https://github.com/ericlight/CN1Extensions/releases/tag/1.0) alone or download [CodenameOne Toolbox](https://ericlight.github.io/CN1Toolbox) which contains this tool and another tool ([CodenameOne Linguist](https://ericlight.github.io/CN1Linguist)).

### Features :
- Explore all libraries
- Search a library using its name, tag or author name
- Download library
- Add/Install a library in a project. **The refresh libs part is done automatically if you have ANT in your environment path**
- Remove/Uninstall a library from a project. **The refresh libs part is done automatically if you have ANT in your environment path. <font color="#d9534f">It will not remove the build hints added by the library. You have to do this part manually if it's required</font>**
- Get a view of all your projects that use a CN1LIB
- Get all libraries that are used in a selected project
- 2 user interface theme (Native and Fusion)

### Roadmap :
- **Submit a library directly from the app**
- **Get notification in the app when a new version of a library is available**
- Build for Mac OS
- Build for Linux
- Dark theme for the user interface
- Translate the app in others languages - I need help for that because i can only speak french and english

### Requirements :
- Windows PC - I'am a Windows user so this [first release](https://github.com/ericlight/CN1Extensions/releases/tag/1.0) of Codename One Extensions is built for Windows. The source code is portable and cross-platform (written in C++/Qt with a little part in Go) so if you are a Mac OS or Linux user, please clone the source code on GitHub and contribute by building it for Mac OS and Linux users. I can help you with some instructions to do it if you want. Check the Contributions section for more informations. Thanks :)

### Video tutorial :
![Image](screenshots/tutorial.jpg "")

### Screenshots :
![Image](screenshots/s1.png "")

![Image](screenshots/s2.png "")

### Author :
My name is Eric, i use Codename One to develop mobile apps since the beginning and i'am also the author of the first book (written in french) on Codename One. 
I still to believe this framework deserves more visibility and users so i decide to build complete and easy-to-use tools to simplify the life of Codename One developers. 
To send me a message, please use [codeureric@gmail.com](mailto:codeureric@gmail.com)

### Contributions :
You can suggest a feature here on GitHub. Just open an RFE and explain your feature idea. You can also open an issue if you find a bug or contribute to the translation of this tool in your language.

Need help to build the Mac OS and Linux versions. The source code is portable and is ready to be built. If you are interested to help in this way, please read the following instructions:

To compile the source code, you will need to download the QtCreator IDE which already contains the Qt framework. Choose the Mac OS or Linux version and help with a build for this platform. Here are the links for the download:
- http://download.qt.io/archive/qt/5.14/5.14.2/ (recommended to get the 5.14 offline installer version)
- https://www.qt.io/offline-installers

Once QtCreator installed in your computer, clone the source code of the project and open the ".pro" file to open the project and to compile. 
If you want me to send me a message, you can do it on [codeureric@gmail.com](mailto:codeureric@gmail.com). Thanks


