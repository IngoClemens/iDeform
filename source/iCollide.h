// ---------------------------------------------------------------------
//
//  iCollide.h
//  iDeform
//
//  Created by ingo on 4/24/14.
//  Copyright (c) 2021 Ingo Clemens. All rights reserved.
//
// ---------------------------------------------------------------------

#ifndef __iDeform__iCollide__
#define __iDeform__iCollide__

#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include <maya/MAnimControl.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MColorArray.h>
#include <maya/MDagPath.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVector.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MMatrix.h>
#include <maya/MMeshIntersector.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPointArray.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MRampAttribute.h>
#include <maya/MThreadUtils.h>
#include <maya/MTime.h>
#include <maya/MTypeId.h>

class iCollide : public MPxDeformerNode
{
public:
    iCollide();
    ~iCollide();

    static void* creator();
    static MStatus initialize();

    void postConstructor();
    MStatus postConstructor_init_curveRamp(MObject& nodeObj,
                                           MObject& rampObj,
                                           int index,
                                           float position,
                                           float value,
                                           int interpolation);

    MStatus postConstructor_init_colorRamp(MObject& nodeObj,
                                           MObject& rampObj,
                                           int index,
                                           float position,
                                           MColor value,
                                           int interpolation);

    MStatus deform(MDataBlock& data,
                   MItGeometry& iter,
                   const MMatrix& mat,
                   unsigned int multiIndex);

    MPoint groundCollisionDelta(unsigned int index,
                                const MMatrix& mat,
                                bool original);
    MStatus setPointFromCollisionCommon(unsigned int index,
                                        MPoint delta,
                                        MPoint deltaOriginal,
                                        float weight);
    MStatus setPointInTraceModeCommon(unsigned int index);
    MStatus setBulge(unsigned int index,
                     double distance,
                     double strength,
                     MVector normal,
                     float weight,
                     const MMatrix& mat);
    MStatus limitGroundBulge(unsigned int index, const MMatrix& mat);
    MStatus setColorFromValue(unsigned int index,
                              float colorPosition,
                              float colorStrength);

    static MTypeId id;

    MRampAttribute curveAttr;
    MRampAttribute colorAttr;
    MRampAttribute colorCurveAttr;

    double bulgeVal;
    float colorGainVal;
    double groundHeightVal;
    int colorModeVal;
    float decayVal;
    MColor defaultColorVal;
    double maxDisplacementVal;
    double radiusVal;
    double scaleFactorVal;
    bool useColorsVal;
    bool useGroundVal;

    float env;

    MPointArray deformPoints;
    MPointArray originalPoints;
    MIntArray collisions;
    MColorArray colorList;
    MColorArray colorListPrev;
    MIntArray colorIndexList;

public:
    // attributes
    static MObject bulge;
    static MObject collider;
    static MObject colliderOffsetMesh;
    static MObject colorCurveRamp;
    static MObject colorGain;
    static MObject colorMode;
    static MObject colorRamp;
    static MObject curveRamp;
    static MObject decay;
    static MObject defaultColor;
    static MObject groundHeight;
    static MObject intersectionRange;
    static MObject maxDisplacement;
    static MObject maxRange;
    static MObject mode;
    static MObject offset;
    static MObject parentMatrix;
    static MObject radius;
    static MObject result;
    static MObject scaleFactor;
    static MObject startFrame;
    static MObject useColors;
    static MObject useGround;
    static MObject useSmooth;
    static MObject useStartFrame;
};

#endif

// ---------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2021 Ingo Clemens, brave rabbit
// iDeform, iCollide, iDisplace, iSkinDeform are under the terms of the
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Author: Ingo Clemens    www.braverabbit.com
// ---------------------------------------------------------------------
