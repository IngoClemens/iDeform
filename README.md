# iDeform
Deformer suite for Autodesk Maya.

**Important Note**
The new version 2.x contains significant code changes since the last official release (1.5.1).
It is possible that the results of a scene made with an older version of the plug-in are different from the deformations
produced by version 2.x. It is recommended to finish older scenes with the previous version of the plug-in and use the updated version only with new projects.

**iCollide, iDisplace and iSkinDeform are under the terms of the MIT License**

### iCollide
A collision deformer which provides simple collision effects without the need for simulations.
iCollide offers simple ground collisions as well as with other mesh objects. The deformation can be shaped with a number of attributes and regular curve controls via the attribute editor. The deformation can be either flexible or even kept after the collision for permanent deformation effects. Color controls optionally apply vertex coloring based on the various deformation attributes.
The plug-in has been inspired by the StretchMesh Deformer made by Kickstand.

Tutorial on Vimeo: [iCollide](https://vimeo.com/55876041)

### iDisplace
A displacement deformer with the ability to use locators or curves to define the area of influence as a dropoff control. The dropoff function is furthermore defined through a curve ramp in the attribute editor.
Since the 2015 release of Maya and it’s own texture deformer iDisplace has almost become obsolete. However, iDisplace still features dropoff points and curves, which makes it the more versatile choice if better control is needed. Also nested textures are usually problematic when using the texture deformer.

Tutorial on Vimeo: [iDisplace](https://vimeo.com/55511098)

Tutorial on Vimeo: [Vertex color based displacement](https://vimeo.com/157510194)

### iSkinDeform
This deformer allows to add skin sliding as well as skin attraction effects to a mesh. Skin sliding can be controlled through a control object and keeps the original mesh shape while deforming the vertices over the existing surface. Skin attraction can be useful for applying sticky effects to the mesh and can be used for lips of a character.
iSkinDeform is deformer-based version of the slide components tool available with the Maya Bonus Tools.

Tutorial on Vimeo: [iSkinDeform](https://vimeo.com/55880201)

## Installation

For ease of use all files are combined as a module. This allows for an easy installation and keeps all necessary files in one location.

**_Important_**

**_If after the installation the menu items don't show up it's possible that the downloaded files from github have faulty user permissions. In this case try to create the modules folder manually and check the permissions or download the file from [braverabbit](http://www.braverabbit.com/ideform/)._**

Copy the module folder from the repository to your Maya preferences. The suite is version independent which means it can be installed in the preferences root folder.

The Maya preferences root directory is located at:

    Windows: C:\Users\USERNAME\Documents\maya
    macOS: /Users/USERNAME/Library/Preferences/Autodesk/maya
    Linux: /home/USERNAME/maya

A default Maya installation doesn't have a modules folder at this specified path. You can directly use the folder from the repository. If the modules folder already exists copy the contents of the repository's modules folder to the one in your preferences.

Inside the modules folder, rename the module template file, which matches your operating system, by removing the current extension. The file should be named iDeform.mod.

Edit the file in a text editor and replace USERNAME in the paths with your user name. Save the file.

Restart Maya and find the iDeform menu in the main menu bar at the top of the interface.

## Usage

When properly installed the main Maya menu contains a new menu item named iDeform. You can create and edit the deformers in a similar way you would with standard deformers in Maya.

### Latest version: 2.0.0 (2018-11-10)


## Changelog

**2.0.0 (2018-11-10)**

    - Initial open source release.
    - The suite now only contains pre-compiled plug-ins for Maya versions 2016.5 and later.
    - The deformers are now multithreaded. The only exception is the sampling of NURBS curves when used as influences.
    - The deformers are now playing back correctly when the evaluation mode in the Maya preferences is set to parallel.
    - iCollide bug (fixed): Uneven surfaces produce a wrong collision vector.
    - iCollide issue (fixed): It's not possible create the deformer with only one mesh selected.
    - iDisplace/iSkinDeform: Fixed several interpolation issues when using multiple influences.
    - Separated the scripts into individual files for better management.
    - Added icons for the menu and the nodes in the outliner and node editor.
    - Added a new command to the menu (Connect Transform) to connect a transform to the iCollide deformer in case additional translation is needed.

    Important attribute changes:
    - iCollide: Removed:
        - Random
        - Seed
        - AccelerationRange
        - ThresholdFactor
    - iCollide: Added:
        - MaxRange: The maximum distance the deformer searches for collision surfaces.
        - IntersectionRange: The distance in world units for the maximum allowed intersection. Keep small if the mesh is in enclosed spaces.
    - iDisplace: Removed:
        - UseColors
    - iDisplace: Added:
        - AverageColors: If active, the texture colors from all channels get averaged. When inactive the color channels directly modify the displacement vector (RGB > XYZ).
