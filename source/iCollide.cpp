// ---------------------------------------------------------------------
//
//  iCollide.cpp
//  iDeform
//
//  Created by ingo on 4/24/14.
//  Copyright (c) 2021 Ingo Clemens. All rights reserved.
//
// ---------------------------------------------------------------------

#include "iCollide.h"

#include <cmath>


MTypeId iCollide::id(0x0011C1C2);


// input
MObject iCollide::collider;
MObject iCollide::parentMatrix;
// controls
MObject iCollide::bulge;
MObject iCollide::colliderOffsetMesh;
MObject iCollide::curveRamp;
MObject iCollide::decay;
MObject iCollide::groundHeight;
MObject iCollide::intersectionRange;
MObject iCollide::maxRange;
MObject iCollide::mode;
MObject iCollide::offset;
MObject iCollide::radius;
MObject iCollide::startFrame;
MObject iCollide::useGround;
MObject iCollide::useStartFrame;
// colors
MObject iCollide::colorCurveRamp;
MObject iCollide::colorGain;
MObject iCollide::colorMode;
MObject iCollide::colorRamp;
MObject iCollide::defaultColor;
MObject iCollide::maxDisplacement;
MObject iCollide::result;
MObject iCollide::scaleFactor;
MObject iCollide::useColors;
MObject iCollide::useSmooth;


// ---------------------------------------------------------------------
// creator
// ---------------------------------------------------------------------

iCollide::iCollide()
{}

iCollide::~iCollide()
{}

void* iCollide::creator()
{
    return new iCollide();
}


// ---------------------------------------------------------------------
// initialize the attributes
// ---------------------------------------------------------------------

MStatus iCollide::initialize()
{
    //
    // MFnEnumAttribute
    //

    MFnEnumAttribute eAttr;

    colorMode = eAttr.create("colorMode", "cm", 0);
    eAttr.addField("Displacement", 0);
    eAttr.addField("Radius", 1);
    eAttr.addField("Bulge", 2);

    mode = eAttr.create("mode", "m", 0);
    eAttr.addField("Relax", 0);
    eAttr.addField("Trace", 1);
    eAttr.setKeyable(true);

    result = eAttr.create("result", "res", 0);
    eAttr.addField("Deformation And Color", 0);
    eAttr.addField("Color Only", 1);

    //
    // MFnNumericAttribute
    //

    MFnNumericAttribute nAttr;

    bulge = nAttr.create("bulge", "bul", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(10);

    colorGain = nAttr.create("colorGain", "cg", MFnNumericData::kFloat);
    nAttr.setDefault(1.0);
    nAttr.setMin(0);
    nAttr.setSoftMax(5);

    decay = nAttr.create("decay", "dec", MFnNumericData::kFloat);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);

    defaultColor = nAttr.createColor("defaultColor", "dc");
    nAttr.setDefault(1.0, 1.0, 1.0);

    groundHeight = nAttr.create("groundHeight", "gh", MFnNumericData::kDouble);
    nAttr.setDefault(0.0);
    nAttr.setSoftMin(-10);
    nAttr.setSoftMax(10);
    nAttr.setKeyable(true);

    intersectionRange = nAttr.create("intersectionRange", "ir", MFnNumericData::kDouble);
    nAttr.setDefault(5.0);
    nAttr.setMin(0);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(10);
    nAttr.setKeyable(true);

    maxDisplacement = nAttr.create("maxDisplacement", "md", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    nAttr.setMin(0);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(5);

    maxRange = nAttr.create("maxRange", "mr", MFnNumericData::kFloat);
    nAttr.setDefault(10.0);
    nAttr.setMin(0);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(100);
    nAttr.setKeyable(true);

    offset = nAttr.create("offset", "of", MFnNumericData::kDouble);
    nAttr.setDefault(0.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(-5);
    nAttr.setSoftMax(5);

    radius = nAttr.create("radius", "rad", MFnNumericData::kDouble);
    nAttr.setDefault(5.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(50);

    scaleFactor = nAttr.create("scaleFactor", "dsf", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    nAttr.setMin(0);

    useSmooth = nAttr.create("smooth", "sm", MFnNumericData::kBoolean);
    nAttr.setDefault(false);

    useColors = nAttr.create("useColors", "uc", MFnNumericData::kBoolean);
    nAttr.setDefault(false);

    useGround = nAttr.create("useGround", "ug", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    useStartFrame = nAttr.create("useStartFrame", "ustf", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    startFrame = nAttr.create("startFrame", "stf", MFnNumericData::kLong);
    nAttr.setDefault(1);
    nAttr.setKeyable(true);

    //
    // MFnMatrixAttribute
    //

    MFnMatrixAttribute mAttr;

    parentMatrix = mAttr.create("parentMatrix", "pm");
    mAttr.setHidden(true);

    //
    // MRampAttribute
    //

    MRampAttribute rAttr;

    curveRamp = rAttr.createCurveRamp("bulgeShape", "bsh");

    colorRamp = rAttr.createColorRamp("colorGradient", "cgr");

    colorCurveRamp = rAttr.createCurveRamp("colorStrength", "cst");

    //
    // MFnTypedAttribute
    //

    MFnTypedAttribute tAttr;

    collider = tAttr.create("collider", "col", MFnData::kMesh);
    tAttr.setHidden(true);
    tAttr.setArray(true);

    colliderOffsetMesh = tAttr.create("colliderOffsetMesh", "com", MFnData::kMesh);
    tAttr.setHidden(true);
    tAttr.setUsesArrayDataBuilder(true);

    // -----------------------------------------------
    // add attributes (order matters)
    // -----------------------------------------------

    addAttribute(collider);
    addAttribute(colliderOffsetMesh);
    addAttribute(offset);
    addAttribute(bulge);
    addAttribute(radius);
    addAttribute(maxRange);
    addAttribute(intersectionRange);
    addAttribute(useGround);
    addAttribute(groundHeight);
    addAttribute(mode);
    addAttribute(useStartFrame);
    addAttribute(startFrame);
    addAttribute(decay);
    addAttribute(curveRamp);

    addAttribute(useColors);
    addAttribute(result);
    addAttribute(colorMode);
    addAttribute(maxDisplacement);
    addAttribute(scaleFactor);
    addAttribute(defaultColor);
    addAttribute(useSmooth);
    addAttribute(colorGain);
    addAttribute(colorRamp);
    addAttribute(colorCurveRamp);

    addAttribute(parentMatrix);

    // -----------------------------------------------
    // affects
    // -----------------------------------------------

    attributeAffects(iCollide::bulge, iCollide::outputGeom);
    attributeAffects(iCollide::collider, iCollide::outputGeom);
    attributeAffects(iCollide::colliderOffsetMesh, iCollide::outputGeom);
    attributeAffects(iCollide::colorCurveRamp, iCollide::outputGeom);
    attributeAffects(iCollide::colorGain, iCollide::outputGeom);
    attributeAffects(iCollide::colorMode, iCollide::outputGeom);
    attributeAffects(iCollide::colorRamp, iCollide::outputGeom);
    attributeAffects(iCollide::curveRamp, iCollide::outputGeom);
    attributeAffects(iCollide::decay, iCollide::outputGeom);
    attributeAffects(iCollide::defaultColor, iCollide::outputGeom);
    attributeAffects(iCollide::groundHeight, iCollide::outputGeom);
    attributeAffects(iCollide::intersectionRange, iCollide::outputGeom);
    attributeAffects(iCollide::maxDisplacement, iCollide::outputGeom);
    attributeAffects(iCollide::maxRange, iCollide::outputGeom);
    attributeAffects(iCollide::mode, iCollide::outputGeom);
    attributeAffects(iCollide::offset, iCollide::outputGeom);
    attributeAffects(iCollide::parentMatrix, iCollide::outputGeom);
    attributeAffects(iCollide::radius, iCollide::outputGeom);
    attributeAffects(iCollide::result, iCollide::outputGeom);
    attributeAffects(iCollide::scaleFactor, iCollide::outputGeom);
    attributeAffects(iCollide::startFrame, iCollide::outputGeom);
    attributeAffects(iCollide::useColors, iCollide::outputGeom);
    attributeAffects(iCollide::useGround, iCollide::outputGeom);
    attributeAffects(iCollide::useSmooth, iCollide::outputGeom);
    attributeAffects(iCollide::useStartFrame, iCollide::outputGeom);

    return MS::kSuccess;
}


// ---------------------------------------------------------------------
// initialize the ramp attribute
// ---------------------------------------------------------------------

void iCollide::postConstructor()
{
    MStatus status;
    MObject thisNode = this->thisMObject();

    // One entry is the least needed or the attribute editor will
    // produce an error.

    postConstructor_init_curveRamp(thisNode, curveRamp, 0, 0.0f, 0.0f, 3);
    postConstructor_init_curveRamp(thisNode, curveRamp, 1, 0.2f, 1.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRamp, 2, 1.0f, 0.0f, 2);

    postConstructor_init_curveRamp(thisNode, colorCurveRamp, 0, 0.0f, 0.0f, 3);
    postConstructor_init_curveRamp(thisNode, colorCurveRamp, 1, 0.3f, 0.7f, 3);
    postConstructor_init_curveRamp(thisNode, colorCurveRamp, 2, 0.7f, 1.0f, 3);

    postConstructor_init_colorRamp(thisNode, colorRamp, 0, 0.0f, MColor(1.0f, 0.0f, 0.0f), 1);
    postConstructor_init_colorRamp(thisNode, colorRamp, 1, 0.5f, MColor(0.0f, 1.0f, 0.0f), 1);
    postConstructor_init_colorRamp(thisNode, colorRamp, 2, 1.0f, MColor(0.0f, 0.0f, 1.0f), 1);

    MPxDeformerNode::setDeformationDetails(MPxDeformerNode::kDeformsColors);
}

MStatus iCollide::postConstructor_init_curveRamp(MObject& nodeObj,
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

MStatus iCollide::postConstructor_init_colorRamp(MObject& nodeObj,
                                                 MObject& rampObj,
                                                 int index,
                                                 float position,
                                                 MColor value,
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
    status = valuePlug.child(0).setFloat(value.r);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = valuePlug.child(1).setFloat(value.g);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = valuePlug.child(2).setFloat(value.b);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MPlug interpolationPlug = elementPlug.child(2);
    interpolationPlug.setInt(interpolation);

    return MS::kSuccess;
}


// ---------------------------------------------------------------------
// deformer function
// ---------------------------------------------------------------------

MStatus iCollide::deform(MDataBlock& data,
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

    MDataHandle bulgeData = data.inputValue(bulge, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bulgeVal = bulgeData.asDouble();

    MArrayDataHandle colliderArrayDataHandle = data.inputArrayValue(collider, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle colliderOffsetMeshHandle = data.inputValue(colliderOffsetMesh, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle colorGainData = data.inputValue(colorGain, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    colorGainVal = colorGainData.asFloat();

    MDataHandle colorModeData = data.inputValue(colorMode, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    colorModeVal = colorModeData.asShort();

    MDataHandle decayData = data.inputValue(decay, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    decayVal = decayData.asFloat();

    MDataHandle defaultColorData = data.inputValue(defaultColor, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MFloatVector &defaultColorValue = defaultColorData.asFloatVector();
    defaultColorVal = MColor(defaultColorValue.x, defaultColorValue.y, defaultColorValue.z, 1.0);

    MDataHandle groundHeightData = data.inputValue(groundHeight, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    groundHeightVal = groundHeightData.asDouble();

    MDataHandle intersectionRangeData = data.inputValue(intersectionRange, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    double intersectionRangeVal = intersectionRangeData.asDouble() * -1;

    MDataHandle maxDisplacementData = data.inputValue(maxDisplacement, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    maxDisplacementVal = maxDisplacementData.asDouble();

    MDataHandle maxRangeData = data.inputValue(maxRange, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    float maxRangeVal = maxRangeData.asFloat();

    MDataHandle modeData = data.inputValue(mode, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int modeVal = modeData.asShort();

    MDataHandle offsetData = data.inputValue(offset, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    double offsetVal = offsetData.asDouble();

    MDataHandle parentMatrixData = data.inputValue(parentMatrix, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MMatrix parentMat = parentMatrixData.asMatrix();

    MDataHandle radiusData = data.inputValue(radius, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    radiusVal = radiusData.asDouble();

    MDataHandle resultData = data.inputValue(result, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int resultVal = resultData.asShort();

    MDataHandle scaleFactorData = data.inputValue(scaleFactor, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    scaleFactorVal = scaleFactorData.asDouble();

    MDataHandle startFrameData = data.inputValue(startFrame, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int startFrameVal = startFrameData.asInt();

    MDataHandle useColorsData = data.inputValue(useColors, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    useColorsVal = useColorsData.asBool();

    MDataHandle useGroundData = data.inputValue(useGround, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    useGroundVal = useGroundData.asBool();

    MDataHandle useSmoothData = data.inputValue(useSmooth, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useSmoothVal = useSmoothData.asBool();

    MDataHandle useStartFrameData = data.inputValue(useStartFrame, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useStartFrameVal = useStartFrameData.asBool();

    // get the ramp attribute

    curveAttr = MRampAttribute(thisNode, curveRamp, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    colorAttr = MRampAttribute(thisNode, colorRamp, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    colorCurveAttr = MRampAttribute(thisNode, colorCurveRamp, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // get the deformer envelope

    MDataHandle envData = data.inputValue(envelope, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    env = envData.asFloat();

    // Don't continue if the deformer envelope is off.
    if (env == 0)
        return MStatus::kSuccess;

    // -----------------------------------------------------------------
    // get the current frame
    // -----------------------------------------------------------------

    MTime frame = MAnimControl::currentTime();
    double frameVal = frame.value();

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
        MGlobal::displayWarning("iCollide works only with polygonal objects.");
        return MStatus::kFailure;
    }

    MObject meshObj = inputGeomHandle.asMesh();
    MFnMesh meshFn(meshObj);
    MMeshIntersector intersector;
    MFloatVectorArray normals;
    meshFn.getVertexNormals(true, normals);

    unsigned vtxCount = (unsigned)iter.count();

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
    // Get a reference to the output mesh for setting the vertex colors.
    MFnMesh outMeshFn(outputDataHandle.asMesh());

    // -----------------------------------------------------------------
    // get the collider meshes
    // -----------------------------------------------------------------

    MDataHandle colliderDataHandle;
    MIntArray colliderIndices;

    // Go through the collider array handle and check for any connected
    // meshes.
    for (i = 0; i < colliderArrayDataHandle.elementCount(); i ++)
    {
        // get the next element in the data array
        colliderArrayDataHandle.jumpToElement(i);
        unsigned int elementIndex = colliderArrayDataHandle.elementIndex();
        colliderDataHandle = colliderArrayDataHandle.inputValue(&status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        // check if the collider geometry is a mesh
        if (colliderDataHandle.type() != MFnData::kMesh)
            return MStatus::kFailure;

        // Some data array handle indices can be empty because they have
        // been disconnected. Therefore it's necessary to check for a
        // connection to verify which indices are in use.
        MPlug colliderPlug(thisNode, collider);
        if (colliderPlug.elementByLogicalIndex(elementIndex).isConnected())
            colliderIndices.append((int)elementIndex);
    }

    // -----------------------------------------------------------------
    // store the mesh points and check for the deformation mode
    // -----------------------------------------------------------------

    // Fill the colorList with default values if it hasn't been
    // initialized yet.
    if (colorList.length() == 0)
    {
        // If previous colors have been set use these to populate the
        // list. Previous colors exist in case color smoothing is
        // enabled or when the mode is set to Trace.
        // The previous colors are always unsmoothed.
        if (colorListPrev.length() != 0)
            colorList.copy(colorListPrev);
        else
            colorList.copy(MColorArray(vtxCount, defaultColorVal));
    }
    // If Trace mode and using colors is enabled copy the previous
    // colors to the color list.
    else if (modeVal == 1 && useColorsVal)
        colorList.copy(colorListPrev);

    // Reset the deformation, if:
    // - no deformation has been calculated yet
    // - the vertex count of the mesh changed
    // - the mode is set to Relax
    // - the mode is set to Trace and the current frame is the start
    //   frame in case the start frame should be used
    if (deformPoints.length() == 0 ||
        deformPoints.length() != vtxCount ||
        modeVal == 0 ||
        (modeVal == 1 && useStartFrameVal && startFrameVal == frameVal) ||
        resultVal)
    {
        iter.allPositions(deformPoints, MSpace::kWorld);

        // Only reset the colors when in Relax mode or when at the start
        // frame in Trace mode. This way the color trace can also work
        // with animated meshes even though the displacement gets
        // discarded.
        if (modeVal == 0 || (modeVal == 1 && useStartFrameVal && startFrameVal == frameVal))
        {
            // Create an array containing all vertex indices for setting
            // all vertex colors. Updating the current vertex index list
            // is also necessary when the number of vertices changes due
            // to history edits.
            if (colorIndexList.length() == 0 || colorIndexList.length() != vtxCount)
            {
                colorIndexList.copy(MIntArray(vtxCount, 0));

                tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                                  [&](tbb::blocked_range<unsigned int> r)
                {
                    for (unsigned int k = r.begin(); k < r.end(); k ++)
                        colorIndexList[k] = (int)k;
                });
            }

            // Set the vertex colors in case the colors should be used.
            if (useColorsVal)
            {
                colorList.copy(MColorArray(vtxCount, defaultColorVal));
                meshFn.setVertexColors(colorList, colorIndexList);
            }
        }
    }

    // Store the original point data for correctly calculating the
    // displacement amount. This is not necessary for the Relax mode but
    // rather the Trace mode because the displacement needs to get
    // calculated based on the original point position and not the
    // already deformed points.
    if (originalPoints.length() == 0 || originalPoints.length() != vtxCount)
        iter.allPositions(originalPoints, MSpace::kWorld);

    // Clear the collision count of all previous collisions.
    collisions = MIntArray(vtxCount, 0);

    // -----------------------------------------------------------------
    //
    // ground collision
    //
    // -----------------------------------------------------------------

    MTransformationMatrix parentTransMat(parentMat);
    MVector parentTranslate = parentTransMat.getTranslation(MSpace::kWorld);
    MMatrix parentScaleMat = parentTransMat.asScaleMatrix();
    parentTranslate *= parentScaleMat.inverse();

    if (useGroundVal)
    {
        // -------------------------------------------------------------
        // collision test
        // -------------------------------------------------------------

        bool hasCollision = false;
        double collisionStrength = 0.0;
        bool YAxisUp = MGlobal::isYAxisUp();

        // add the offset value to the ground height
        double parentHeightValue = parentTranslate.y;
        if (!YAxisUp)
            parentHeightValue = parentTranslate.z;
        groundHeightVal += offsetVal - parentHeightValue;

        tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                          [&](tbb::blocked_range<unsigned int> r)
        {
            for (unsigned int k = r.begin(); k < r.end(); k ++)
            {
                float weight = weightValue(data, multiIndex, k);

                // Bring the current point into world space in order to
                // check for a gound collision.
                MPoint point = deformPoints[k];
                MPoint worldPoint = point * mat;
                double worldValue = worldPoint.y;
                if (!YAxisUp)
                    worldValue = worldPoint.z;

                if (worldValue < groundHeightVal && weight != 0)
                {
                    // Calculate the displacement for the point.
                    MPoint delta = groundCollisionDelta(k, mat, false);
                    // Convert the displacement amount to a vector and
                    // use the vector length to define how strong the
                    // collision is.
                    // The maximum displacement value defines the
                    // strength of the overall bulging effect and also
                    // affects the color display.
                    MVector deltaVector(delta);
                    double deltaLength = deltaVector.length();
                    if (deltaLength > collisionStrength)
                        collisionStrength = deltaLength;

                    // Calculate the displacement strength based on the
                    // original position which is necessary for the
                    // correct color display.
                    MPoint deltaOriginal = groundCollisionDelta(k, mat, true);

                    // Set the deformation and the color.
                    setPointFromCollisionCommon(k, delta, deltaOriginal, weight);

                    hasCollision = true;
                }
                //
                // Trace mode
                //
                else if (modeVal == 1)
                {
                    setPointInTraceModeCommon(k);
                }
            }
        });

        // -------------------------------------------------------------
        // bulge effect
        // -------------------------------------------------------------

        // The bulging needs to get evaluated in a separate pass just
        // because of the overall collision strength which is only known
        // when all points have been processed.
        if (hasCollision && bulgeVal != 0)
        {
            tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                              [&](tbb::blocked_range<unsigned int> r)
            {
                for (unsigned int k = r.begin(); k < r.end(); k ++)
                {
                    float weight = weightValue(data, multiIndex, k);

                    bool exists = false;
                    // If in Trace mode skip all points which previously
                    // encontered a collision.
                    if (modeVal == 1 && collisions[k] > 0)
                        exists = true;

                    // Only do the deformation if the weight is not zero.
                    if (weight != 0 && bulgeVal != 0 && exists == false)
                    {
                        // Bring the stored point position into world
                        // space.
                        MPoint worldPoint = deformPoints[k] * mat;

                        double worldValue = worldPoint.y;
                        if (!YAxisUp)
                            worldValue = worldPoint.z;

                        // Get the points distance to the nearest point
                        // on the collision surface.
                        double distance = std::abs(groundHeightVal - worldValue);

                        // If the distance is within the defined radius
                        // the bulging can be calculated.
                        if (distance <= radiusVal)
                            setBulge(k, distance, collisionStrength, normals[k], weight, mat);
                    }
                    //
                    // Trace mode
                    //
                    else if (modeVal == 1)
                    {
                        setPointInTraceModeCommon(k);
                    }
                }
            });
        }
    }

    // -----------------------------------------------------------------
    //
    // collider based deformation
    //
    // -----------------------------------------------------------------

    MObject colliderObj;
    MFnMesh colliderFn;

    for (i = 0; i < colliderIndices.length(); i ++)
    {
        unsigned int colIndex = (unsigned)colliderIndices[i];

        // get the next element in the data array
        colliderArrayDataHandle.jumpToElement(colIndex);
        colliderDataHandle = colliderArrayDataHandle.inputValue(&status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        // define the collider mesh
        colliderObj = colliderDataHandle.asMesh();
        MFnMesh colliderFn(colliderObj);

        // -------------------------------------------------------------
        // create the offset mesh
        // -------------------------------------------------------------

        if (offsetVal != 0)
        {
            MPointArray colliderOffsetPoints;
            colliderFn.getPoints(colliderOffsetPoints, MSpace::kWorld);
            MFloatVectorArray colliderNormals;
            colliderFn.getVertexNormals(true, colliderNormals);

            unsigned int numColPnts = colliderOffsetPoints.length();

            tbb::parallel_for(tbb::blocked_range<unsigned int>(0, numColPnts),
                              [&](tbb::blocked_range<unsigned int> r)
            {
                for (unsigned int k = r.begin(); k < r.end(); k ++)
                {
                    MVector normal = colliderNormals[k];
                    normal.normalize();
                    MPoint point = colliderOffsetPoints[k] + (normal * offsetVal);
                    colliderOffsetPoints.set(point, k);
                }
            });

            // Copy the collider data to the collider offset data handle.
            colliderOffsetMeshHandle.copy(colliderDataHandle);

            MObject colliderOffsetObj = colliderOffsetMeshHandle.asMesh();
            MFnMesh colliderOffsetFn(colliderOffsetObj);

            // Set the offset positions for the offset mesh.
            colliderOffsetFn.setPoints(colliderOffsetPoints, MSpace::kWorld);

            // Replace the original collider with the offset collider.
            colliderFn.setObject(colliderOffsetObj);
            colliderObj = colliderOffsetObj;
        }

        // -------------------------------------------------------------
        // get the collider matrix
        // -------------------------------------------------------------

        MDagPath colliderDagPath = colliderFn.dagPath();
        MMatrix colliderMatrix = colliderDagPath.inclusiveMatrix();
        colliderMatrix *= parentMat;

        // -------------------------------------------------------------
        // create the intersector
        // -------------------------------------------------------------

        status = intersector.create(colliderObj, colliderMatrix);
        MMeshIsectAccelParams mmAccelParams = colliderFn.autoUniformGridParams();
        if (!status)
        {
            MGlobal::displayWarning("Can't create intersector.");
            return status;
        }

        // -------------------------------------------------------------
        // collision test
        // -------------------------------------------------------------

        bool hasCollision = false;
        double collisionStrength = 0.0;

        tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                          [&](tbb::blocked_range<unsigned int> r)
        {
            for (unsigned int k = r.begin(); k < r.end(); k ++)
            {
                float weight = weightValue(data, multiIndex, k);

                // Only calculate the collisions if the weight is not
                // zero.
                if (weight != 0)
                {
                    // Bring the current point into world space in order
                    // to check for a collision.
                    MPoint point = deformPoints[k];
                    MPoint worldPoint = point * mat;

                    MFloatVector rayDirection;
                    MPoint delta;

                    // define the source point for the collision ray
                    MFloatPoint raySource(worldPoint);

                    // Determining a collision involves two steps:
                    // 1. Get the closest point on the collision surface.
                    //    With this point the collision vector can be
                    //    calculated, which is then used for finding the
                    //    intersections with the collider mesh.
                    // 2. Find all intersections with the collider mesh.
                    //    This is necessary to evaluate if there is an
                    //    active collision.

                    // -------------------------------------------------
                    // get the closest point
                    // -------------------------------------------------

                    MPointOnMesh meshPoint;
                    intersector.getClosestPoint(worldPoint, meshPoint, maxRangeVal);
                    // convert the closestPoint (MPointOnMesh) to MPoint
                    MPoint collisionPoint = meshPoint.getPoint();

                    // Get the ray direction for the intersection
                    // calculation based on the mesh point and the
                    // closest point on the collider mesh.
                    rayDirection = collisionPoint - worldPoint;
                    rayDirection.normalize();

                    // Because the collision intersection ray goes into
                    // both directions it's important to filter what's
                    // in front of the surface or behind it.
                    // The previous closest point calculation only
                    // returns a point which can be:
                    //      1. int front of the surface is there is no
                    //         collision yet.
                    //      2. behind the surface if there is a
                    //         collision.
                    // Depending on the case the intersection ray can
                    // start away from the surface or towards the
                    // surface resulting in changing signs for the found
                    // collision distances:
                    // One time a found collision has a positive sign,
                    // the other time it's negative. Just because in
                    // which direction the ray started first.
                    // Therefore the vertex normal helps in finding if
                    // the closest point lies in front or behind the
                    // surface. The dot product of the normal and the
                    // ray is positive if the point lies in front of the
                    // surface.

                    if (rayDirection * normals[k] < 0)
                    {
                        rayDirection = worldPoint - collisionPoint;
                        rayDirection.normalize();
                    }

                    MFloatPointArray hitPoints; // The array of
                                                // collision points.
                    MFloatArray hitRayParams; // The array of the ray
                                              // lengths of the
                                              // intersections.

                    // Find all intersections and return the number of
                    // hits.
                    bool foundIntersect;
                    foundIntersect = colliderFn.allIntersections(raySource,
                                                                 rayDirection,
                                                                 NULL,
                                                                 NULL,
                                                                 true,
                                                                 MSpace::kWorld,
                                                                 maxRangeVal,
                                                                 true,
                                                                 &mmAccelParams,
                                                                 true,
                                                                 hitPoints,
                                                                 &hitRayParams,
                                                                 NULL,
                                                                 NULL,
                                                                 NULL,
                                                                 NULL);

                    // -------------------------------------------------
                    // check for a collision and transform the point
                    // -------------------------------------------------

                    unsigned int intersectCount = hitPoints.length();

                    // Single surfaces are ignored because it's easier
                    // to calculate the intersection if objects are
                    // enclosed. Therefore a ray usually intersects the
                    // collider mesh at least two times.
                    if (foundIntersect && intersectCount >= 2)
                    {
                        int hitIndex = -1;

                        // Go through the number of intersections and
                        // calculate the direction based on pairs
                        // through multiplication:
                        // - Surfaces behind the point are returned as
                        //   negative numbers; the result is positive.
                        // - If the point is in front of the surface the
                        //   result is positive.
                        // - If the point is between surfaces the result
                        //   is negative and a collision must be
                        //   calculated.
                        for (unsigned l = 0; l < intersectCount - 1; l ++)
                        {
                            if (hitRayParams[l] > intersectionRangeVal &&
                                hitRayParams[l + 1] > intersectionRangeVal &&
                                hitRayParams[l] * hitRayParams[l + 1] < 0)
                            {
                                hitIndex = (int)l;
                                break;
                            }
                        }
                        if (hitIndex != -1)
                        {
                            // The intersector works in collider space.
                            // Therefore the resulting collision point
                            // has to be in collider space as well to
                            // include possible scaling of the collider
                            // mesh.
                            collisionPoint *= colliderMatrix;

                            // Exclude the matrix from the mesh object
                            // to make the collision point local, since
                            // the original point position is also local.
                            collisionPoint *= mat.inverse();
                            collisionPoint *= parentMat.inverse();

                            // Calculate the displacement for the point.
                            delta = collisionPoint - point;
                            // Convert the displacement amount to a
                            // vector and use the vector length to
                            // define how strong the collision is.
                            // The maximum displacement value defines
                            // the strength of the overall bulging
                            // effect and also affects the color display.
                            MVector deltaVector(delta);
                            double deltaLength = deltaVector.length();
                            if (deltaLength > collisionStrength)
                                collisionStrength = deltaLength;

                            // Calculate the displacement strength based
                            // on the original position which is
                            // necessary for the correct color display.
                            MPoint deltaOriginal = collisionPoint - originalPoints[k];

                            // Set the deformation and the color.
                            setPointFromCollisionCommon(k, delta, deltaOriginal, weight);

                            hasCollision = true;
                        }
                    }
                    //
                    // Trace mode
                    //
                    else if (modeVal == 1)
                    {
                        setPointInTraceModeCommon(k);
                    }
                }
            }
        });

        // -------------------------------------------------------------
        // bulge effect
        // -------------------------------------------------------------

        // The bulging needs to get evaluated in a separate pass just
        // because of the overall collision strength which is only known
        // when all points have been processed.
        if (hasCollision && bulgeVal != 0)
        {
            tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                              [&](tbb::blocked_range<unsigned int> r)
            {
                for (unsigned int k = r.begin(); k < r.end(); k ++)
                {
                    float weight = weightValue(data, multiIndex, k);

                    bool exists = false;
                    // If in Trace mode skip all points which previously
                    // encontered a collision.
                    if (modeVal == 1 && collisions[k] > 0)
                        exists = true;

                    // Only do the deformation if the weight is not zero.
                    if (weight != 0 && bulgeVal != 0 && exists == false)
                    {
                        // Bring the stored point position into world
                        // space.
                        MPoint worldPoint = deformPoints[k] * mat;

                        // Search for the closest point within the bulge
                        // radius.
                        MPointOnMesh meshPoint;
                        MStatus closestPointStatus = intersector.getClosestPoint(worldPoint,
                                                                                 meshPoint,
                                                                                 radiusVal);

                        // Continue if a closest point was found.
                        if (closestPointStatus)
                        {
                            // Convert the MPointOnMesh to MPoint.
                            MPoint collisionPoint = meshPoint.getPoint();
                            // The intersector works in collider space.
                            collisionPoint *= colliderMatrix;
                            // Get the points distance to the nearest
                            // point on the collision surface.
                            double distance = collisionPoint.distanceTo(worldPoint);

                            // If the distance is within the defined
                            // radius the bulging can be calculated.
                            if (distance <= radiusVal)
                                setBulge(k, distance, collisionStrength, normals[k], weight, mat);
                        }
                    }
                    //
                    // Trace mode
                    //
                    else if (modeVal == 1)
                    {
                        setPointInTraceModeCommon(k);
                    }
                }
            });
        }
    }

    // -----------------------------------------------------------------
    // smooth out the vertex colors
    // -----------------------------------------------------------------

    // store the unsmoothed colors
    if (useColorsVal)
        colorListPrev.copy(colorList);

    if (useColorsVal && useSmoothVal)
    {
        MColorArray averagedColorList(vtxCount, MColor(0.0f));

        tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                          [&](tbb::blocked_range<unsigned int> r)
        {
            MItMeshVertex vtxIter(meshObj);

            for (unsigned int k = r.begin(); k < r.end(); k ++)
            {
                int prevIndex;
                vtxIter.setIndex((int)k, prevIndex);

                MIntArray connected;
                vtxIter.getConnectedVertices(connected);

                MColor vertColor;
                MColor averageColor = colorList[k];
                for (unsigned l = 0; l < connected.length(); l ++)
                {
                    vertColor = colorList[(unsigned)connected[l]];
                    averageColor += vertColor;
                }
                averageColor = averageColor / (float)(connected.length() + 1);

                averagedColorList.set(averageColor, k);
            }
        });

        // copy the averaged colors to the color array
        colorList.copy(averagedColorList);
        averagedColorList.clear();
    }

    // -----------------------------------------------------------------
    // apply the deformation and colors
    // -----------------------------------------------------------------

    if (useColorsVal)
    {
        if (resultVal == 0)
            outIter.setAllPositions(deformPoints);

        // Set the vertex colors for the out mesh object. It's important
        // that the out mesh is used here so that the vertex colors are
        // actually visible.
        outMeshFn.setVertexColors(colorList, colorIndexList);
    }
    else
        outIter.setAllPositions(deformPoints);

    // Important for the EM evaluation.
    // Notify the EM that the mesh has been processed.
    data.setClean(outputGeomPlug);
    // Pass the outputDataHandle.asMesh() object (represented through
    // outMeshFn.object()) to the output data handle.
    outputDataHandle.setMObject(outMeshFn.object());

    return status;
}

//
// Description:
//      Calculate the collision delta for the ground collision.
//
// Input Arguments:
//      index               The vertex index.
//      mat                 The deformed mesh matrix.
//      original            True, if the calculation should be performed
//                          with the original points.
//
// Return Value:
//      MPoint              The collision delta MPoint.
//
MPoint iCollide::groundCollisionDelta(unsigned int index,
                                      const MMatrix& mat,
                                      bool original)
{
    MPoint point;
    if (!original)
        point = deformPoints[index];
    else
        point = originalPoints[index];

    MPoint collisionPoint = point * mat;
    if (MGlobal::isYAxisUp())
        collisionPoint.y = groundHeightVal;
    else
        collisionPoint.z = groundHeightVal;
    collisionPoint *= mat.inverse();

    // return the displacement delta for the point
    return collisionPoint - point;
}

//
// Description:
//      Set the deformed point position based on the given delta MPoint
//      and set the vertex color. Also, store the index of the vertex
//      in the collision indices array.
//
// Input Arguments:
//      index               The vertex index.
//      delta               The collision delta as MPoint.
//      deltaOriginal       The collision delta from the original
//                          mesh point as MPoint.
//      weight              The deformer weight for the vertex.
//
// Return Value:
//      MStatus             (unused)
//
MStatus iCollide::setPointFromCollisionCommon(unsigned int index,
                                              MPoint delta,
                                              MPoint deltaOriginal,
                                              float weight)
{
    MStatus status = MS::kSuccess;

    MVector deltaOriginalVector(deltaOriginal);
    float colorStrength = (float)(deltaOriginalVector.length() /
                                  (maxDisplacementVal * scaleFactorVal + 0.00001));
    if (colorStrength > 1.0)
        colorStrength = 1.0;

    // set the deformation
    deformPoints.set(deformPoints[index] + delta * weight, index);

    // increment the collision count for vertex
    collisions.set(collisions[index] + 1, index);

    if (useColorsVal)
    {
        float colorPosition;
        // Color mode 0 : Displacement
        if (colorModeVal == 0)
            colorPosition = colorStrength;
        // Color mode 1 : Radius
        // Color mode 2 : Bulge
        else
            colorPosition = 0;

        // Set the color based on the deformation.
        setColorFromValue(index, colorPosition, colorStrength);
    }

    return status;
}

//
// Description:
//      Apply the displacement to the already existing deformation for
//      the Trace mode.
//
// Input Arguments:
//      index               The vertex index.
//
// Return Value:
//      MStatus             (unused)
//
MStatus iCollide::setPointInTraceModeCommon(unsigned int index)
{
    MStatus status = MS::kSuccess;

    MPoint point = deformPoints[index] * decayVal + originalPoints[index] * (1 - decayVal);
    deformPoints.set(point, index);

    if (useColorsVal)
    {
        MColor color = colorList[index] * decayVal;
        if (decayVal > 1 && (color.r > 1 || color.g > 1 || color.b > 1))
            color = colorList[index];
        colorList.set(color, index);
    }

    return status;
}

//
// Description:
//      Calculate and apply the bulge displacement for the collided
//      vertex. This also affects the vertex colors.
//
// Input Arguments:
//      index               The vertex index.
//      distance            The distance to the collider.
//      strength            The maximum displacement value.
//      normal              The MVector of the normal at the collision
//                          point.
//      weight              The deformer weight for the vertex.
//      mat                 The matrix of the deformed mesh.
//
// Return Value:
//      MStatus             (unused)
//
MStatus iCollide::setBulge(unsigned int index,
                           double distance,
                           double strength,
                           MVector normal,
                           float weight,
                           const MMatrix& mat)
{
    MStatus status = MS::kSuccess;

    // Normalize the distance based on the radius attribute.
    float distanceNormalized = (float)(distance / (radiusVal + 0.00001));
    if (distanceNormalized > 1.0)
        distanceNormalized = 1.0;

    // Get the amount of bulging based on the curve ramp.
    float value;
    curveAttr.getValueAtPosition(distanceNormalized, value);

    // Normalize the collisionStrength based on the radius attribute.
    float strengthNormalized = (float)(strength / (radiusVal + 0.00001));
    if (strengthNormalized > 1.0)
        strengthNormalized = 1.0;

    // Calculate the modified point position and add it to the list.
    MVector bulgeVector = normal * value * (bulgeVal / 4) * strengthNormalized;
    deformPoints.set(deformPoints[index] + bulgeVector * env * weight, index);

    // Depending on the normal direction of the vertex it's possible
    // that the resulting point position from the bulge penetrates the
    // ground. In this case flatten the point to prevent an
    // intersection.
    if (useGroundVal && bulgeVal != 0)
        limitGroundBulge(index, mat);

    if (useColorsVal)
    {
        // get the color from the color ramp
        if (colorModeVal != 0)
        {
            float colorPosition = 0;
            // Color mode 1 : Radius
            if (colorModeVal == 1)
                colorPosition = distanceNormalized;
            // Color mode 2 : Bulge
            else if (colorModeVal == 2)
                colorPosition = value;

            // Set the color based on the deformation.
            setColorFromValue(index, colorPosition, strengthNormalized);
        }
    }

    return status;
}

//
// Description:
//      Limit the deformation from the bulge effect to prevent any
//      intersections with the ground.
//
// Input Arguments:
//      index               The vertex index.
//      mat                 The matrix of the deformed mesh.
//
// Return Value:
//      MStatus             (unused)
//
MStatus iCollide::limitGroundBulge(unsigned int index, const MMatrix& mat)
{
    MStatus status = MS::kSuccess;

    MPoint point = deformPoints[index];
    MPoint worldPoint = point * mat;
    double worldValue = worldPoint.y;
    if (!MGlobal::isYAxisUp())
        worldValue = worldPoint.z;

    if (worldValue < groundHeightVal)
    {
        MPoint delta = groundCollisionDelta(index, mat, false);
        deformPoints.set(deformPoints[index] + delta, index);
    }

    return status;
}

//
// Description:
//      Calculate the final vertex color with the given color position
//      and strength and the color ramp attributes.
//
// Input Arguments:
//      index               The vertex index.
//      colorPosition       The displacement amount to represent as
//                          a color.
//      colorStrength       The color amound based on the collision
//                          radius.
//
// Return Value:
//      MStatus             (unused)
//
MStatus iCollide::setColorFromValue(unsigned int index,
                                    float colorPosition,
                                    float colorStrength)
{
    MStatus status = MS::kSuccess;

    // Get the color from the color ramp.
    MColor color;
    colorAttr.getColorAtPosition(colorPosition, color);

    // Get the amount of color blending based on the
    // color curve ramp.
    float colorValue;
    colorCurveAttr.getValueAtPosition(colorStrength, colorValue);

    // Create a mix between the ramp color and the
    // default color based on the collisionStrength.
    color = color * colorValue + (defaultColorVal * (1 - colorValue));
    color *= colorGainVal;
    colorList.set(color, index);

    return status;
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
