**2.0.0 (2022-06-01) - Update**
* Added support for Maya 2023.

**2.0.0 (2021-03-29) - Update**
* Added support for Maya 2022.

**2.0.0 (2020-03-07) - Update**
* Added support for Maya 2020.

**2.0.0 (2018-11-10)**
* Initial open source release.
* The suite now only contains pre-compiled plug-ins for Maya versions 2016.5 and later.
* The deformers are now multithreaded. The only exception is the sampling of NURBS curves when used as influences.
* The deformers are now playing back correctly when the evaluation mode in the Maya preferences is set to parallel.
* iCollide bug (fixed): Uneven surfaces produce a wrong collision vector.
* iCollide issue (fixed): It's not possible create the deformer with only one mesh selected.
* iDisplace/iSkinDeform: Fixed several interpolation issues when using multiple influences.
* Separated the scripts into individual files for better management.
* Added icons for the menu and the nodes in the outliner and node editor.
* Added a new command to the menu (Connect Transform) to connect a transform to the iCollide deformer in case additional translation is needed.
* Important attribute changes:
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
