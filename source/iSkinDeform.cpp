// ---------------------------------------------------------------------
//
//  iSkinDeform.cpp
//  iDeform
//
//  Created by ingo on 4/24/14.
//  Copyright (c) 2021 Ingo Clemens. All rights reserved.
//
// ---------------------------------------------------------------------

#include "iSkinDeform.h"


MTypeId iSkinDeform::id(0x0011C1C3);


MObject iSkinDeform::attractor;
MObject iSkinDeform::attractorCurve;
MObject iSkinDeform::curveRamp;
MObject iSkinDeform::curveRampUVal;
MObject iSkinDeform::influence;
MObject iSkinDeform::method;
MObject iSkinDeform::negativeClamp;
MObject iSkinDeform::pinch;
MObject iSkinDeform::radius;
MObject iSkinDeform::restPosition;
MObject iSkinDeform::strength;
MObject iSkinDeform::useSliding;


// ---------------------------------------------------------------------
// creator
// ---------------------------------------------------------------------

iSkinDeform::iSkinDeform()
{}

iSkinDeform::~iSkinDeform()
{}

void* iSkinDeform::creator()
{
    return new iSkinDeform();
}


// ---------------------------------------------------------------------
// initialize the attributes
// ---------------------------------------------------------------------

MStatus iSkinDeform::initialize()
{
    //
    // MFnEnumAttribute
    //

    MFnEnumAttribute eAttr;

    attractor = eAttr.create("attractor", "atc", 0);
    eAttr.addField("Point", 0);
    eAttr.addField("Curve (Slow)", 1);
    eAttr.setKeyable(true);

    method = eAttr.create("method", "sm", 0);
    eAttr.addField("ClosestPoint", 0);
    eAttr.addField("Raycast", 1);
    eAttr.setDefault(1);
    eAttr.setKeyable(true);

    //
    // MFnNumericAttribute
    //

    MFnNumericAttribute nAttr;

    negativeClamp = nAttr.create("negativeClamp", "nc", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    pinch = nAttr.create("pinch", "pi", MFnNumericData::kDouble);
    nAttr.setDefault(0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);
    nAttr.setMax(1);

    radius = nAttr.create("radius", "rad", MFnNumericData::kFloat);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(50);

    restPosition = nAttr.createPoint("restPositionList", "rpl");
    nAttr.setArray(true);

    useSliding = nAttr.create("sliding", "sl", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    strength = nAttr.create("strength", "st", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(10);

    //
    // MFnMatrixAttribute
    //

    MFnMatrixAttribute mAttr;

    influence = mAttr.create("influenceMatrix", "cm");
    mAttr.setHidden(true);
    mAttr.setArray(true);

    //
    // MFnTypedAttribute
    //

    MFnTypedAttribute tAttr;

    attractorCurve = tAttr.create("attractorCurve", "ag", MFnData::kNurbsCurve);
    tAttr.setHidden(true);
    tAttr.setArray(true);

    //
    // MRampAttribute
    //

    MRampAttribute rAttr;

    curveRampUVal = rAttr.createCurveRamp("curveFalloff", "cfo");

    curveRamp = rAttr.createCurveRamp("falloff", "fo");

    // -----------------------------------------------
    // add attributes (order matters)
    // -----------------------------------------------

    addAttribute(influence);
    addAttribute(attractorCurve);
    addAttribute(strength);
    addAttribute(radius);
    addAttribute(attractor);
    addAttribute(pinch);
    addAttribute(negativeClamp);
    addAttribute(useSliding);
    addAttribute(method);
    addAttribute(curveRamp);
    addAttribute(curveRampUVal);
    addAttribute(restPosition);

    // -----------------------------------------------
    // affects
    // -----------------------------------------------

    attributeAffects(iSkinDeform::attractor, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::attractorCurve, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::curveRamp, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::curveRampUVal, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::influence, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::method, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::negativeClamp, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::pinch, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::radius, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::restPosition, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::strength, iSkinDeform::outputGeom);
    attributeAffects(iSkinDeform::useSliding, iSkinDeform::outputGeom);

    return MS::kSuccess;
}


// ---------------------------------------------------------------------
// initialize the ramp attribute
// ---------------------------------------------------------------------

void iSkinDeform::postConstructor()
{
    MStatus status;
    MObject thisNode = this->thisMObject();

    postConstructor_init_curveRamp(thisNode, curveRamp, 0, 0.0f, 1.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRamp, 1, 1.0f, 0.0f, 2);

    postConstructor_init_curveRamp(thisNode, curveRampUVal, 0, 0.0f, 0.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRampUVal, 1, 0.5f, 1.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRampUVal, 2, 1.0f, 0.0f, 2);
}

MStatus iSkinDeform::postConstructor_init_curveRamp(MObject& nodeObj,
                                                    MObject& rampObj,
                                                    int index,
                                                    float position,
                                                    float value,
                                                    int interpolation)
{
    MStatus status;

    MPlug rampPlug(nodeObj, rampObj);
    MPlug elementPlug = rampPlug.elementByLogicalIndex((unsigned)index, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MPlug positionPlug = elementPlug.child(0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = positionPlug.setFloat(position);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MPlug valuePlug = elementPlug.child(1);
    status = valuePlug.setFloat(value);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MPlug interpolationPlug = elementPlug.child(2);
    interpolationPlug.setInt(interpolation);

    return MS::kSuccess;
}


// ---------------------------------------------------------------------
// deformer function
// ---------------------------------------------------------------------

MStatus iSkinDeform::deform(MDataBlock& data,
                            MItGeometry& iter,
                            const MMatrix& mat,
                            unsigned int multiIndex)
{
    MStatus status = MS::kSuccess;

    MObject thisNode = this->thisMObject();

    unsigned int i;

    // -----------------------------------------------------------------
    // deformer attributes
    // -----------------------------------------------------------------

    MDataHandle attractorData = data.inputValue(attractor, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int attractorVal = attractorData.asShort();

    MArrayDataHandle attrCurveArrayDataHandle = data.inputArrayValue(attractorCurve, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MArrayDataHandle influenceArrayDataHandle = data.inputArrayValue(influence, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MArrayDataHandle restPositionArrayDataHandle = data.inputArrayValue(restPosition, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle methodData = data.inputValue(method, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int methodVal = methodData.asShort();

    MDataHandle useClampData = data.inputValue(negativeClamp, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool negativeClampVal = useClampData.asBool();

    MDataHandle pinchData = data.inputValue(pinch, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    double pinchVal = pinchData.asDouble();

    MDataHandle radiusData = data.inputValue(radius, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    radiusVal = radiusData.asFloat();

    MDataHandle strengthData = data.inputValue(strength, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    double strengthVal = strengthData.asDouble();

    MDataHandle useSlidingData = data.inputValue(useSliding, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useSlidingVal = useSlidingData.asBool();

    // get the ramp attribute

    curveAttr = MRampAttribute(thisNode, curveRamp, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MRampAttribute curveAttrUVal(thisNode, curveRampUVal, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // get the deformer envelope

    MDataHandle envData = data.inputValue(envelope, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    float env = envData.asFloat();

    if (env == 0)
        return MStatus::kSuccess;

    // -----------------------------------------------------------------
    // get the input mesh
    // -----------------------------------------------------------------

    MArrayDataHandle inputArrayHandle = data.outputArrayValue(input, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = inputArrayHandle.jumpToArrayElement(multiIndex);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // check if the input geometry is a mesh
    MDataHandle inputGeomHandle = inputArrayHandle.outputValue().child(inputGeom);
    if (inputGeomHandle.type() != MFnData::kMesh)
    {
        MGlobal::displayWarning("iDisplace works only with polygonal objects.");
        return MStatus::kFailure;
    }

    MObject meshObj = inputGeomHandle.asMesh();
    MFnMesh meshFn(meshObj);
    MMeshIntersector intersector;

    // -----------------------------------------------------------------
    // Get the output mesh which is important for a proper evaluation
    // with the Evaluation Manager.
    // -----------------------------------------------------------------

    MPlug outputGeomPlug(thisNode, outputGeom);
    outputGeomPlug.selectAncestorLogicalIndex(multiIndex, outputGeom);

    // Create a unique copy of the input data for the output data.
    MDataHandle outputDataHandle = data.outputValue(outputGeomPlug);
    outputDataHandle.copyWritable(inputGeomHandle);

    // Create a new MItGeometry iterator instead of using the one
    // provided through the deform method.
    // This is a crucial step for making the deformer work in DG as well
    // as parallel mode because when using the method iterator the
    // deformation doesn't update in parallel mode.
    // The reason for this is not yet clear as the documentation doesn't
    // contain the necessary information and the devkit examples don't
    // elaborate on this either.
    MDataHandle groupData = inputGeomHandle.child(groupId);
    unsigned int groupId = (unsigned)groupData.asInt();
    MItGeometry outIter(outputDataHandle, groupId, false);

    // The mesh points in local space. These are the points needed to
    // do the displacement calculation.
    MPointArray meshPoints;
    meshFn.getPoints(meshPoints);
    MFloatVectorArray normals;
    meshFn.getVertexNormals(true, normals);

    unsigned vtxCount = (unsigned)iter.count();

    // -----------------------------------------------------------------
    // create the intersector for the slide mode
    // -----------------------------------------------------------------

    MMeshIsectAccelParams mmAccelParams = meshFn.autoUniformGridParams();

    if (useSlidingVal && attractorVal == 0)
    {
        status = intersector.create(meshObj, mat);
        if (!status)
        {
            MGlobal::displayWarning("Can't create intersector.");
            return status;
        }
    }

    // -----------------------------------------------------------------
    // get the influences
    // -----------------------------------------------------------------

    MPointArray influencePositions;
    MPointArray restPositions;
    MIntArray curveIndices;

    // Go through the influence array handle and check for connections.
    for (i = 0; i < influenceArrayDataHandle.elementCount(); i ++)
    {
        // Some data array handle indices can be empty because they have
        // been disconnected. Therefore it's necessary to check for a
        // connection to verify which indices are in use.
        MPlug plugInfluence(thisNode, influence);
        if (plugInfluence.elementByLogicalIndex(i).isConnected())
        {
            // ---------------------------------------------------------
            // transform influences
            // ---------------------------------------------------------
            if (attractorVal == 0)
            {
                // get the next element in the data array
                influenceArrayDataHandle.jumpToElement(i);
                MDataHandle influenceDataHandle = influenceArrayDataHandle.inputValue(&status);
                CHECK_MSTATUS_AND_RETURN_IT(status);

                unsigned int index = plugInfluence.elementByLogicalIndex(i).logicalIndex();

                if (!restPositionArrayDataHandle.elementCount())
                {
                    MGlobal::displayWarning(MString("No rest position defined for influence id ") + index);
                    return MStatus::kFailure;
                }

                // get the influence rest positions
                restPositionArrayDataHandle.jumpToElement(i);
                MDataHandle restPositionDataHandle = restPositionArrayDataHandle.inputValue(&status);
                CHECK_MSTATUS_AND_RETURN_IT(status);
                MFloatVector restPos = restPositionDataHandle.asFloatVector();
                restPositions.append(restPos);

                // get the influence matrices
                MMatrix influenceMatrix = influenceDataHandle.asMatrix();
                MTransformationMatrix influenceTransMatrix(influenceMatrix);
                influencePositions.append(influenceTransMatrix.getTranslation(MSpace::kWorld));
            }
            // ---------------------------------------------------------
            // curve influences
            // ---------------------------------------------------------
            else if (!useSlidingVal)
            {
                // get the next element in the data array
                attrCurveArrayDataHandle.jumpToElement(i);
                MDataHandle attrCurveDataHandle = attrCurveArrayDataHandle.inputValue(&status);
                CHECK_MSTATUS_AND_RETURN_IT(status);

                // Some data array handle indices can be empty because
                // they have been disconnected. Therefore it's necessary
                // to check for a connection to verify which indices are
                // in use.
                MPlug plugAttractor(thisNode, attractorCurve);
                if (plugAttractor.elementByLogicalIndex(i).isConnected() &&
                    attrCurveDataHandle.type() == MFnData::kNurbsCurve)
                {
                    curveIndices.append((int)i);
                }
            }
        }
    }

    // -----------------------------------------------------------------
    //
    // transform influence based deformation
    //
    // -----------------------------------------------------------------

    if (attractorVal == 0)
    {
        tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                          [&](tbb::blocked_range<unsigned int> r)
        {
            for (unsigned int k = r.begin(); k < r.end(); k ++)
            {
                float weight = weightValue(data, multiIndex, k);

                if (weight != 0)
                {
                    MPoint point = meshPoints[k];
                    MPoint worldPoint = point * mat;
                    MVector normal = normals[k];

                    double distance = 0.0;
                    MVector minDelta;
                    unsigned int closestInfluenceIndex = 0;

                    // Go through all transform influences and check
                    // which is the closest to the point.
                    for (unsigned l = 0; l < influencePositions.length(); l ++)
                    {
                        // Get the influence position or rest position.
                        MPoint deltaPoint = influencePositions[l];
                        if (useSlidingVal)
                            deltaPoint = restPositions[l];

                        MVector delta = deltaPoint - worldPoint;
                        double length = delta.length();
                        if (l == 0)
                        {
                            distance = length;
                            minDelta = delta;
                        }
                        else
                        {
                            if (length < distance)
                            {
                                distance = length;
                                minDelta = delta;
                                closestInfluenceIndex = l;
                            }
                        }
                    }

                    MPoint resultPoint(0.0, 0.0, 0.0, 1.0);

                    if (distance <= radiusVal)
                    {
                        // Read the amount of displacement based on the
                        // curve ramp.
                        float value = getDisplaceValue(distance);

                        // avoid a negative transformation if requested
                        if (negativeClampVal)
                            distance = clampVector(minDelta, normal);

                        // ---------------------------------------------
                        // attractor mode
                        // ---------------------------------------------

                        if (!useSlidingVal)
                        {
                            // Blend between the normal and the delta
                            // vector for attracting the points to the
                            // influence. The normal needs to get scaled
                            // by the normalized distance to avoid
                            // overshooting.
                            normal = normal * distance * (1 - pinchVal) + minDelta * pinchVal;

                            // set the new point position
                            MFloatVector result;
                            result = normal * strengthVal * value * env * weight;
                            resultPoint = worldPoint + result;
                            resultPoint *= mat.inverse();
                        }

                        // ---------------------------------------------
                        // slide mode
                        // ---------------------------------------------

                        else
                        {
                            // Get the displacement strength from the
                            // influences offset positions.
                            MPoint influencePoint = influencePositions[closestInfluenceIndex];
                            MVector inflDelta = influencePoint - restPositions[closestInfluenceIndex];

                            // The delta between the points needs to be
                            // in local space.
                            inflDelta *= mat.inverse();

                            // Calculate the new point position based on
                            // the influence offset.
                            MFloatVector result;
                            result = inflDelta * strengthVal * value * env * weight;
                            MPoint movedPoint = point + result;

                            // Bring the point into world space for the
                            // closest point calculations.
                            movedPoint *= mat;

                            // -----------------------------------------
                            // raycast mode
                            // -----------------------------------------

                            if (methodVal == 1)
                            {
                                // Define the source point for the
                                // collision ray.
                                MFloatPoint raySource(movedPoint);
                                MFloatVector rayDirection = normal;

                                MFloatPoint hitPoint;

                                // Find all intersections and return the
                                // number of hits.
                                bool foundIntersect = false;
                                foundIntersect = meshFn.closestIntersection(raySource,
                                                                            rayDirection,
                                                                            NULL,
                                                                            NULL,
                                                                            true,
                                                                            MSpace::kWorld,
                                                                            radiusVal,
                                                                            true,
                                                                            &mmAccelParams,
                                                                            hitPoint,
                                                                            NULL,
                                                                            NULL,
                                                                            NULL,
                                                                            NULL,
                                                                            NULL);

                                if (foundIntersect)
                                {
                                    resultPoint = hitPoint;
                                    resultPoint *= mat.inverse();
                                }
                            }

                            // -----------------------------------------
                            // closestPoint mode
                            // -----------------------------------------

                            else
                            {
                                MPointOnMesh meshPoint;
                                MStatus closestPointStatus = intersector.getClosestPoint(movedPoint,
                                                                                         meshPoint,
                                                                                         radiusVal);
                                // Continue if a closest point was found.
                                if (closestPointStatus)
                                {
                                    // Convert the closestPoint
                                    // (MPointOnMesh) to MPoint.
                                    resultPoint = meshPoint.getPoint();
                                }
                            }
                        }

                        meshPoints.set(resultPoint, k);
                    }
                }
            }
        });
    }

    // -----------------------------------------------------------------
    //
    // curve influence based deformation
    //
    // Evaluating and deforming with influence curves is done in a
    // separate pass because nurbs curve evaluation is not supposed to
    // be thread-safe.
    // -----------------------------------------------------------------
    else
    {
        for (i = 0; i < curveIndices.length(); i ++)
        {
            // get the next element in the data array
            attrCurveArrayDataHandle.jumpToElement((unsigned)curveIndices[i]);
            MDataHandle attrCurveDataHandle = attrCurveArrayDataHandle.inputValue(&status);
            CHECK_MSTATUS_AND_RETURN_IT(status);

            MObject curveObj = attrCurveDataHandle.asNurbsCurve();
            MFnNurbsCurve curveFn(curveObj);

            for (iter.reset(); !iter.isDone(); iter.next())
            {
                unsigned int vertIndex = (unsigned)iter.index();

                float weight = weightValue(data, multiIndex, vertIndex);

                if (weight != 0)
                {
                    MPoint point = meshPoints[vertIndex];
                    MPoint worldPoint = point * mat;
                    MVector normal = normals[vertIndex];

                    double curveParam;
                    MPoint curvePoint = curveFn.closestPoint(worldPoint, &curveParam, 0.00001,
                                                             MSpace::kWorld);

                    MVector delta = curvePoint - worldPoint;
                    double distance = delta.length();

                    MPoint resultPoint(0.0, 0.0, 0.0, 1.0);

                    if (distance <= radiusVal)
                    {
                        // Read the amount of displacement based on the
                        // curve ramp.
                        float value = getDisplaceValue(distance);

                        // avoid a negative transformation if requested
                        if (negativeClampVal)
                            distance = clampVector(delta, normal);

                        // The closestPoint function only returns param
                        // values between 0 and 1. If the curve has a
                        // greater min max range the values need to get
                        // remapped.
                        double curveStart;
                        double curveEnd;
                        curveFn.getKnotDomain(curveStart, curveEnd);

                        // Get the amount of displacement based on the
                        // curveFalloff ramp.
                        float curveValue;
                        curveAttrUVal.getValueAtPosition((float)(curveParam / curveEnd),
                                                         curveValue);

                        value *= curveValue;

                        // Blend between the normal and the delta vector
                        // for attracting the points to the influence.
                        // The normal needs to get scaled by the
                        // normalized distance to avoid overshooting.
                        normal = normal * distance * (1 - pinchVal) + delta * pinchVal;

                        // set the new point position
                        MFloatVector result;
                        result = normal * strengthVal * value * env * weight;
                        resultPoint = worldPoint + result;
                        resultPoint *= mat.inverse();

                        meshPoints.set(resultPoint, vertIndex);
                    }
                }
            }
        }
    }

    outIter.setAllPositions(meshPoints);

    // Important for the EM evaluation.
    // Notify the EM that the mesh has been processed.
    data.setClean(outputGeomPlug);
    outputDataHandle.setMObject(outputDataHandle.asMesh());

    return status;
}

//
// Description:
//      Read the amount of displacement based on the curve ramp.
//
// Input Arguments:
//      distance                The distance of the point to the
//                              influence.
//
// Return Value:
//      float                   The resulting color value.
//
float iSkinDeform::getDisplaceValue(double distance)
{
    float value;

    float distanceNormalized = (float)(distance / radiusVal);
    if (distanceNormalized > 1.0)
        distanceNormalized = 1.0;
    curveAttr.getValueAtPosition(distanceNormalized, value);

    return value;
}

//
// Description:
//      Clamp the given delta vector to only contain positive values
//      when the dot product between the two vectors is below zero to
//      prevent any negative transformations.
//
// Input Arguments:
//      delta                   The delta vector between the point and
//                              the influence.
//      normal                  The normal of the vertex.
//
// Return Value:
//      double                  The clamped distance value.
//
double iSkinDeform::clampVector(MVector delta, MVector normal)
{
    if (delta.x * normal.x < 0) delta.x = 0;
    if (delta.y * normal.y < 0) delta.y = 0;
    if (delta.z * normal.z < 0) delta.z = 0;
    return delta.length();
}

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
