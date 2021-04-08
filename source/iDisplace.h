// ---------------------------------------------------------------------
//
//  iDisplace.h
//  iDeform
//
//  Created by ingo on 4/24/14.
//  Copyright (c) 2021 Ingo Clemens. All rights reserved.
//
// ---------------------------------------------------------------------

#ifndef __iDeform__iDisplace__
#define __iDeform__iDisplace__

#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MDagPath.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MRenderUtil.h>
#include <maya/MThreadUtils.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MTypeId.h>
#include <maya/MVectorArray.h>

class iDisplace : public MPxDeformerNode
{
public:
    iDisplace();
    ~iDisplace();

    static void* creator();
    static MStatus initialize();

    void postConstructor();
    MStatus postConstructor_init_curveRamp(MObject& nodeObj,
                                           MObject& rampObj,
                                           int index,
                                           float position,
                                           float value,
                                           int interpolation);

    MStatus deform(MDataBlock& data,
                   MItGeometry& iter,
                   const MMatrix& mat,
                   unsigned int multiIndex);

    float getColorValue(MColor color, int multiplier[4]);
    float getMinDropoff(float value, float dropoff);
    float getDropoffDistance(MPoint point,
                             MPoint dropoffPoint,
                             float uValue,
                             float dropoffValue);

    static MTypeId id;

public:
    // attributes
    static MObject averageColors;
    static MObject colorChannel;
    static MObject colorGain;
    static MObject colorGamma;
    static MObject colorSetName;
    static MObject curveRamp;
    static MObject curveRampUVal;
    static MObject dropoffType;
    static MObject direction;
    static MObject directionX;
    static MObject directionY;
    static MObject directionZ;
    static MObject directionMatrix;
    static MObject influenceCurve;
    static MObject influenceMatrix;
    static MObject influenceMesh;
    static MObject influenceU;
    static MObject invert;
    static MObject local;
    static MObject offset;
    static MObject radius;
    static MObject scale;
    static MObject texture;
    static MObject type;
    static MObject useDropoff;
    static MObject useTexture;
    static MObject useTransform;
    static MObject useUV;

    MRampAttribute curveAttr;
    MRampAttribute curveAttrUVal;
    float colorGainVal;
    float colorGammaVal;
    bool invertVal;
    double radiusVal;
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
