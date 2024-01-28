# Sonic Adventure 2 Battle Mod Template
### A blank mod template for anyone to clone and begin writing their own mod.

To begin coding your own mod, simply clone the repository with the following command:

```git clone https://github.com/X-Hax/SA2B-Mod-Template```

Or by downloading the repository as a zip and working from there.

## How to Use
This repo contains a 'main.cpp' file where you will be doing a majority of your coding. Also contained in the repo is many optional libraries courtesy of the [SA2ModLoader](https://github.com/X-Hax/sa2-mod-loader) which let you better interface with Sonic Adventure 2, allowing you to manipulate game states, objects, characters, score, etc.

Once you've finished making changes to your 'main.cpp' file, simply Build your solution with 'Release x86' selected. Then, a 'Release' folder will generate, containing your mod dll along with some extra files.

To import your mod into Sonic Adventure 2, you will need to create a mod folder along with a mod.ini describing your mod. **An example mod folder** is already provided for you, so simply **drag that into your Sonic Adventure 2 Mods folder**, and any time you build a new dll, drag and drop that new dll into your mods folder. Once done, your mod is ready to play!

For ideas, see the [SA2BModding Wiki](https://github.com/X-Hax/SA2BModdingGuide) for more information on what you can do with mods!

## Dependencies
Some of the libraries included will not automatically update themselves, so please check every so often for updates. You can find the two main dependency libraries here:
- [Libmodutils](https://github.com/X-Hax/sa2-mod-loader/tree/master/libmodutils)
- Programming Folder (Which can be found in your local Sonic Adventure 2 install, after installing the SA2ModLoader)

Note that Libmodutils originally used the legacy 'stdafx.cpp' and 'stdafx.h' instead of the more recent 'pch.cpp' and 'pch.h.' If you update your libraries, please double check and fix the cpp files to ensure that they do include 'pch.h' instead of 'stdafx.h.'

## Notes
The project name 'my-mod' is a placeholder. Feel free to rename it if you'd like, which should also change the dll that gets built.

For a faster workflow, I recommend leaving a shortcut to your mod folder in your 'release' folder. That way, when you build a new mod, you can simply drag and drop
the new dll onto the shortcut, which will quickly update your mod. Shortcuts to your SA2 install folder can also be helpful for other modding purposes.

If you need any help, feel free to ask questions at the official [X-Hax Discord](https://discord.gg/gqJCF47).

Happy programming!
