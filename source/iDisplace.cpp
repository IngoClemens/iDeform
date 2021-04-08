// ---------------------------------------------------------------------
//
//  iDisplace.cpp
//  iDeform
//
//  Created by ingo on 4/24/14.
//  Copyright (c) 2021 Ingo Clemens. All rights reserved.
//
// ---------------------------------------------------------------------

#include "iDisplace.h"


MTypeId iDisplace::id(0x0011C1C0);


// controls
MObject iDisplace::averageColors;
MObject iDisplace::direction;
MObject iDisplace::directionX;
MObject iDisplace::directionY;
MObject iDisplace::directionZ;
MObject iDisplace::directionMatrix;
MObject iDisplace::local;
MObject iDisplace::offset;
MObject iDisplace::scale;
MObject iDisplace::texture;
MObject iDisplace::type;
MObject iDisplace::useTexture;
MObject iDisplace::useTransform;
MObject iDisplace::useUV;
// influences
MObject iDisplace::colorChannel;
MObject iDisplace::colorGain;
MObject iDisplace::colorGamma;
MObject iDisplace::colorSetName;
MObject iDisplace::curveRamp;
MObject iDisplace::curveRampUVal;
MObject iDisplace::dropoffType;
MObject iDisplace::influenceMatrix;
MObject iDisplace::influenceCurve;
MObject iDisplace::influenceMesh;
MObject iDisplace::influenceU;
MObject iDisplace::invert;
MObject iDisplace::radius;
MObject iDisplace::useDropoff;


// ---------------------------------------------------------------------
// creator
// ---------------------------------------------------------------------

iDisplace::iDisplace()
{}

iDisplace::~iDisplace()
{}

void* iDisplace::creator()
{
    return new iDisplace();
}


// ---------------------------------------------------------------------
// initialize the attributes
// ---------------------------------------------------------------------

MStatus iDisplace::initialize()
{
    //
    // MFnEnumAttribute
    //

    MFnEnumAttribute eAttr;

    colorChannel = eAttr.create("colorChannel", "cc", 0);
    eAttr.addField("RGB", 0);
    eAttr.addField("Red", 1);
    eAttr.addField("Green", 2);
    eAttr.addField("Blue", 3);
    eAttr.addField("Alpha", 4);
    eAttr.setDefault(0);
    eAttr.setKeyable(true);

    dropoffType = eAttr.create("dropoffType", "dt", 0);
    eAttr.addField("Point", 0);
    eAttr.addField("Curve (Slow)", 1);
    eAttr.addField("Vertex Color", 2);
    eAttr.setDefault(0);
    eAttr.setKeyable(true);

    type = eAttr.create("type", "t", 0);
    eAttr.addField("Normal", 0);
    eAttr.addField("Placement", 1);
    eAttr.addField("User", 2);
    eAttr.setDefault(0);
    eAttr.setKeyable(true);

    //
    // MFnNumericAttribute
    //

    MFnNumericAttribute nAttr;

    averageColors = nAttr.create("averageColors", "ac", MFnNumericData::kBoolean);
    nAttr.setDefault(true);
    nAttr.setKeyable(true);

    colorGain = nAttr.create("colorGain", "cg", MFnNumericData::kFloat);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(5);

    colorGamma = nAttr.create("colorGamma", "cga", MFnNumericData::kFloat);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(5);

    directionX = nAttr.create("directionX", "dirx", MFnNumericData::kDouble);
    nAttr.setDefault(0.0);
    nAttr.setKeyable(true);

    directionY = nAttr.create("directionY", "diry", MFnNumericData::kDouble);
    nAttr.setDefault(0.0);
    nAttr.setKeyable(true);

    directionZ = nAttr.create("directionZ", "dirz", MFnNumericData::kDouble);
    nAttr.setDefault(-1.0);
    nAttr.setKeyable(true);

    influenceU = nAttr.create("influenceU", "iu", MFnNumericData::kFloat);
    nAttr.setHidden(true);
    nAttr.setArray(true);

    invert = nAttr.create("invert", "inv", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    local = nAttr.create("local", "lc", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    offset = nAttr.create("offset", "of", MFnNumericData::kFloat);
    nAttr.setDefault(0.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(-5);
    nAttr.setSoftMax(5);

    radius = nAttr.create("radius", "rad", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(0);
    nAttr.setSoftMax(10);

    scale = nAttr.create("scale", "sc", MFnNumericData::kDouble);
    nAttr.setDefault(1.0);
    nAttr.setKeyable(true);
    nAttr.setSoftMin(-1);
    nAttr.setSoftMax(10);

    texture = nAttr.createColor("texture", "tx");
    nAttr.setKeyable(true);

    useDropoff = nAttr.create("useDropoff", "ud", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    useTexture = nAttr.create("useTexture", "ut", MFnNumericData::kBoolean);
    nAttr.setDefault(true);
    nAttr.setKeyable(true);

    useTransform = nAttr.create("useTransform", "utr", MFnNumericData::kBoolean);
    nAttr.setDefault(false);
    nAttr.setKeyable(true);

    useUV = nAttr.create("useUV", "uv", MFnNumericData::kBoolean);
    nAttr.setDefault(true);
    nAttr.setKeyable(true);

    //
    // MFnCompoundAttribute
    //

    MFnCompoundAttribute cAttr;

    direction = cAttr.create("direction", "dir");
    cAttr.addChild(directionX);
    cAttr.addChild(directionY);
    cAttr.addChild(directionZ);

    //
    // MFnMatrixAttribute
    //

    MFnMatrixAttribute mAttr;

    directionMatrix = mAttr.create("directionMatrix", "dirm");
    mAttr.setHidden(true);

    influenceMatrix = mAttr.create("influenceMatrix", "im");
    mAttr.setHidden(true);
    mAttr.setArray(true);

    //
    // MFnTypedAttribute
    //

    MFnTypedAttribute tAttr;

    influenceCurve = tAttr.create("influenceCurve", "ic", MFnData::kNurbsCurve);
    tAttr.setHidden(true);
    tAttr.setArray(true);

    influenceMesh = tAttr.create("influenceMesh", "icm", MFnData::kMesh);
    tAttr.setHidden(true);
    tAttr.setArray(false);

    colorSetName = tAttr.create("colorSetName", "csn", MFnData::kString);
    tAttr.setHidden(false);
    tAttr.setArray(false);

    //
    // MRampAttribute
    //

    MRampAttribute rAttr;

    curveRamp = rAttr.createCurveRamp("dropoffCurve", "dc");

    curveRampUVal = rAttr.createCurveRamp("curveFalloff", "cfo");

    // -----------------------------------------------
    // add attributes (order matters)
    // -----------------------------------------------

    addAttribute(texture);
    addAttribute(useTexture);
    addAttribute(averageColors);
    addAttribute(useUV);
    addAttribute(local);
    addAttribute(useTransform);
    addAttribute(scale);
    addAttribute(offset);
    addAttribute(type);
    addAttribute(directionMatrix);
    addAttribute(direction);
    addAttribute(useDropoff);
    addAttribute(dropoffType);
    addAttribute(radius);
    addAttribute(influenceMesh);
    addAttribute(colorSetName);
    addAttribute(invert);
    addAttribute(curveRamp);
    addAttribute(curveRampUVal);
    addAttribute(influenceMatrix);
    addAttribute(influenceCurve);
    addAttribute(influenceU);
    addAttribute(colorChannel);
    addAttribute(colorGain);
    addAttribute(colorGamma);

    // -----------------------------------------------
    // affects
    // -----------------------------------------------

    attributeAffects(iDisplace::averageColors, iDisplace::outputGeom);
    attributeAffects(iDisplace::colorChannel, iDisplace::outputGeom);
    attributeAffects(iDisplace::colorGain, iDisplace::outputGeom);
    attributeAffects(iDisplace::colorGamma, iDisplace::outputGeom);
    attributeAffects(iDisplace::colorSetName, iDisplace::outputGeom);
    attributeAffects(iDisplace::curveRamp, iDisplace::outputGeom);
    attributeAffects(iDisplace::curveRampUVal, iDisplace::outputGeom);
    attributeAffects(iDisplace::direction, iDisplace::outputGeom);
    attributeAffects(iDisplace::directionMatrix, iDisplace::outputGeom);
    attributeAffects(iDisplace::dropoffType, iDisplace::outputGeom);
    attributeAffects(iDisplace::influenceCurve, iDisplace::outputGeom);
    attributeAffects(iDisplace::influenceMatrix, iDisplace::outputGeom);
    attributeAffects(iDisplace::influenceMesh, iDisplace::outputGeom);
    attributeAffects(iDisplace::influenceU, iDisplace::outputGeom);
    attributeAffects(iDisplace::invert, iDisplace::outputGeom);
    attributeAffects(iDisplace::local, iDisplace::outputGeom);
    attributeAffects(iDisplace::offset, iDisplace::outputGeom);
    attributeAffects(iDisplace::radius, iDisplace::outputGeom);
    attributeAffects(iDisplace::scale, iDisplace::outputGeom);
    attributeAffects(iDisplace::texture, iDisplace::outputGeom);
    attributeAffects(iDisplace::type, iDisplace::outputGeom);
    attributeAffects(iDisplace::useDropoff, iDisplace::outputGeom);
    attributeAffects(iDisplace::useTexture, iDisplace::outputGeom);
    attributeAffects(iDisplace::useTransform, iDisplace::outputGeom);
    attributeAffects(iDisplace::useUV, iDisplace::outputGeom);

    return MS::kSuccess;
}


// ---------------------------------------------------------------------
// initialize the ramp attribute
// ---------------------------------------------------------------------

void iDisplace::postConstructor()
{
    MStatus status;
    MObject thisNode = this->thisMObject();

    postConstructor_init_curveRamp(thisNode, curveRamp, 0, 0.0f, 1.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRamp, 1, 1.0f, 0.0f, 2);

    postConstructor_init_curveRamp(thisNode, curveRampUVal, 0, 0.0f, 0.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRampUVal, 1, 0.5f, 1.0f, 2);
    postConstructor_init_curveRamp(thisNode, curveRampUVal, 2, 1.0f, 0.0f, 2);
}

MStatus iDisplace::postConstructor_init_curveRamp(MObject& nodeObj,
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

MStatus iDisplace::deform(MDataBlock& data,
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

    MDataHandle averageColorsData = data.inputValue(averageColors, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool averageColorsVal = averageColorsData.asBool();

    MDataHandle colorChannelData = data.inputValue(colorChannel, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int colorChannelTypeVal = colorChannelData.asShort();

    MDataHandle colorGainData = data.inputValue(colorGain, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    colorGainVal = colorGainData.asFloat();

    MDataHandle colorGammaData = data.inputValue(colorGamma, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    colorGammaVal = colorGammaData.asFloat();

    MDataHandle colorSetNameData = data.inputValue(colorSetName, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MString colorSetNameVal = colorSetNameData.asString();

    MDataHandle dropoffTypeData = data.inputValue(dropoffType, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int dropoffTypeVal = dropoffTypeData.asShort();

    MDataHandle directionData = data.inputValue(direction, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MVector directionVal = directionData.asVector();

    MDataHandle directionMatrixData = data.inputValue(directionMatrix, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MMatrix directionMat = directionMatrixData.asMatrix();

    MArrayDataHandle influenceCurveArrayDataHandle = data.inputArrayValue(influenceCurve, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MArrayDataHandle influenceMatrixArrayDataHandle = data.inputArrayValue(influenceMatrix, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle influenceMeshData = data.inputValue(influenceMesh, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MObject colorMeshObj = influenceMeshData.asMesh();

    MArrayDataHandle influenceUArrayDataHandle = data.inputArrayValue(influenceU, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle invertData = data.inputValue(invert, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    invertVal = invertData.asBool();

    MDataHandle localData = data.inputValue(local, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool localVal = localData.asBool();

    MDataHandle offsetData = data.inputValue(offset, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    float offsetVal = offsetData.asFloat();

    MDataHandle radiusData = data.inputValue(radius, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    radiusVal = radiusData.asDouble();

    MDataHandle scaleData = data.inputValue(scale, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    double scaleVal = scaleData.asDouble();

    MDataHandle textureData = data.inputValue(texture, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MFloatVector textureVec = textureData.asFloatVector();

    MDataHandle typeData = data.inputValue(type, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int typeVal = typeData.asShort();

    MDataHandle useDropoffData = data.inputValue(useDropoff, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useDropoffVal = useDropoffData.asBool();

    MDataHandle useTextureData = data.inputValue(useTexture, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useTextureVal = useTextureData.asBool();

    MDataHandle useTransformData = data.inputValue(useTransform, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useTransformVal = useTransformData.asBool();

    MDataHandle useUVData = data.inputValue(useUV, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    bool useUVVal = useUVData.asBool();

    // get the ramp attribute

    curveAttr = MRampAttribute(thisNode, curveRamp, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    curveAttrUVal = MRampAttribute(thisNode, curveRampUVal, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // get the deformer envelope

    MDataHandle envData = data.inputValue(envelope, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    float env = envData.asFloat();

    // Don't continue if the deformer envelope is off.
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

    // The mesh points in local space. These are the points needed to
    // do the displacement calculation.
    MPointArray meshPoints;
    meshFn.getPoints(meshPoints);

    // The mesh points in world space as float points. These are needed
    // for the texture sampling.
    MFloatPointArray points;
    if (localVal)
        meshFn.getPoints(points, MSpace::kObject);
    else
        meshFn.getPoints(points, MSpace::kWorld);

    // -----------------------------------------------------------------
    // Get the name of the connected shading node and the matrix of the
    // 3d placement node in case of a 3d texture node.
    // -----------------------------------------------------------------

    MString displaceTextureOutput = "";
    MPlug texturePlug(thisNode, texture);
    MMatrix placementMatrix;

    // Get the connected node and attribute for the displacement texture.
    MPlugArray connectedTexture;
    texturePlug.connectedTo(connectedTexture, true, false);
    if (useTextureVal && connectedTexture.length() > 0)
    {
        displaceTextureOutput = connectedTexture[0].name();

        // find the 3d placement node of the connected texture
        MObject textureObj(connectedTexture[0].node());

        const MString matrixAttr("placementMatrix");
        const MString placementMatrixAttr("matrix");

        MFnDependencyNode textureFn(textureObj);
        MPlug placementPlug = textureFn.findPlug(matrixAttr, false, &status);
        if (status)
        {
            MPlugArray connectedPlacement;
            placementPlug.connectedTo(connectedPlacement, true, false);
            if (connectedPlacement.length() > 0)
            {
                MObject placementObj(connectedPlacement[0].node());
                if (placementObj.hasFn(MFn::kPlace3dTexture))
                {
                    MFnDependencyNode placementFn(placementObj);
                    MPlug matrixPlug = placementFn.findPlug(placementMatrixAttr, false, &status);
                    if (status)
                    {
                        MObject matrixObj;
                        matrixPlug.getValue(matrixObj);
                        MFnMatrixData matrixFn(matrixObj, &status);
                        if (status)
                        {
                            placementMatrix = matrixFn.matrix();
                        }
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------
    // collect the vertex color mesh data
    // -----------------------------------------------------------------

    MFnMesh colorMeshFn(colorMeshObj);
    MColorArray colorList;
    colorMeshFn.getVertexColors(colorList, &colorSetNameVal);

    // If colors are present and if the vertex count matches create a
    // multiplier list which is used to scale the displacement based
    // on the color channel amount.
    bool hasColors = false;
    int colorMult[4] = {0, 0, 0, 0};
    if (colorList.length() == vtxCount && useDropoffVal && dropoffTypeVal == 2)
    {
        hasColors = true;

        if (colorChannelTypeVal == 0)
        {
            colorMult[0] = 1;
            colorMult[1] = 1;
            colorMult[2] = 1;
        }
        else if (colorChannelTypeVal == 1)
            colorMult[0] = 1;
        else if (colorChannelTypeVal == 2)
            colorMult[1] = 1;
        else if (colorChannelTypeVal == 3)
            colorMult[2] = 1;
        else if (colorChannelTypeVal == 4)
            colorMult[3] = 1;
    }

    // -----------------------------------------------------------------
    // Get the per vertex data for the texture sampling and the final
    // displacement calculation.
    // -----------------------------------------------------------------

    MFloatVectorArray normals;

    // displacement along normals
    if (typeVal == 0)
    {
        meshFn.getVertexNormals(true, normals);
    }
    else
    {
        MTransformationMatrix transMat;

        double scaleMat[3];
        scaleMat[0] = 1.0;
        scaleMat[1] = 1.0;
        scaleMat[2] = 1.0;

        // texture placement
        if (typeVal == 1)
            transMat = placementMatrix;
        // custom matrix
        else
            transMat = directionMat;

        // Reset the scale component of the transformation matrix.
        transMat.setScale(scaleMat, MSpace::kWorld);

        MMatrix directionMatrix = transMat.asMatrix();

        // Create a normal vector from the given direction vector and
        // the connected matrix.
        MVector normal = directionVal * directionMatrix;
        normals = MFloatVectorArray(vtxCount, normal);
    }

    MFloatArray uCoords;
    MFloatArray vCoords;
    meshFn.getUVs(uCoords, vCoords);

    MFloatArray filterSizes(vtxCount, 0.00001f);

    MFloatVectorArray colors(vtxCount, textureVec);

    MFloatVectorArray transparencies;

    // fill the dropoffs array with default values
    float dropoffDefault = 1.0f;
    if (useDropoffVal)
        dropoffDefault = 1.0f - (int)!invertVal;
    MFloatArray dropoffs(vtxCount, dropoffDefault);

    // -----------------------------------------------------------------
    // get the dropoff locator positions
    // -----------------------------------------------------------------

    MPointArray dropoffPositions;
    MFloatArray inflUValues;

    if (useDropoffVal && dropoffTypeVal == 0)
    {
        // Go through the list of influences and check for connections.
        for (i = 0; i < influenceMatrixArrayDataHandle.elementCount(); i ++)
        {
            // Some data array handle indices can be empty because they
            // have been disconnected. Therefore it's necessary to check
            // for a connection to verify which indices are in use.
            MPlug plugDropoffPosition(thisNode, influenceMatrix);
            if (MPlug(plugDropoffPosition).elementByLogicalIndex(i).isConnected())
            {
                // get the next element in the influence array
                influenceMatrixArrayDataHandle.jumpToElement(i);
                MDataHandle influenceMatrixDataHandle = influenceMatrixArrayDataHandle.inputValue(&status);
                CHECK_MSTATUS_AND_RETURN_IT(status);
                MMatrix dropoffMatrix = influenceMatrixDataHandle.asMatrix();
                MTransformationMatrix dropoffTransformMatrix(dropoffMatrix);
                dropoffPositions.append(dropoffTransformMatrix.getTranslation(MSpace::kWorld));

                // get the next element in the u value array
                influenceUArrayDataHandle.jumpToElement(i);
                MDataHandle influenceUDataHandle = influenceUArrayDataHandle.inputValue(&status);
                CHECK_MSTATUS_AND_RETURN_IT(status);
                inflUValues.append(influenceUDataHandle.asFloat());
            }
        }
    }

    // -----------------------------------------------------------------
    // Calculate the curve dropoff.
    //
    // Evaluating the dropoff curves is done in a separate pass because
    // it's much slower to process than the dropoff locators.
    // Also, nurbs curve evaluation is not supposed to be thread-safe.
    //
    // Each curve is processed separately by evaluating the closest
    // point for every vertex.
    // The downside to this procedure is that the final number of
    // evaluations is numberOfCurves * vertexCount. Though this is much
    // faster than going through the vertex list just once but then
    // having to query and generate data handles for the curves for each
    // and every vertex.
    //
    // There might be room for improvement here but since the evaluation
    // is not thread-safe to begin with it might not be the primary
    // choice for dropoffs anyway.
    // -----------------------------------------------------------------

    if (useDropoffVal && dropoffTypeVal == 1)
    {
        // Go through the list of curve influences and check for
        // connections.
        for (i = 0; i < influenceCurveArrayDataHandle.elementCount(); i ++)
        {
            // get the next element in the curve influence array
            influenceCurveArrayDataHandle.jumpToElement(i);
            MDataHandle influenceCurveDataHandle = influenceCurveArrayDataHandle.inputValue(&status);
            CHECK_MSTATUS_AND_RETURN_IT(status);

            // Some data array handle indices can be empty because they
            // have been disconnected. Therefore it's necessary to check
            // for a connection to verify which indices are in use.
            MPlug plugDropoffCurve(thisNode, influenceCurve);
            if (MPlug(plugDropoffCurve).elementByLogicalIndex(i).isConnected() &&
                influenceCurveDataHandle.type() == MFnData::kNurbsCurve)
            {
                MObject inflCurveObj = influenceCurveDataHandle.asNurbsCurve();
                MFnNurbsCurve curveFn(inflCurveObj);

                for (iter.reset(); !iter.isDone(); iter.next())
                {
                    unsigned int vertIndex = (unsigned)iter.index();

                    MPoint point = points[vertIndex];

                    if (!localVal)
                        point *= mat;

                    double curveParam;
                    MPoint curvePoint = curveFn.closestPoint(point, &curveParam, 0.00001,
                                                             MSpace::kWorld);

                    // calculate the distance to the closest point
                    MVector delta = curvePoint - point;
                    double distance = delta.length();

                    if (distance <= radiusVal)
                    {
                        // Get the amount of displacement based on
                        // the curve ramp.
                        float value;
                        float distanceNormalized = (float)(distance / radiusVal);
                        if (distanceNormalized > 1.0)
                            distanceNormalized = 1.0;
                        curveAttr.getValueAtPosition(distanceNormalized, value);

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

                        dropoffs[vertIndex] = getMinDropoff(value, dropoffs[vertIndex]);
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------
    // sample the connected 2d or 3d textures
    // -----------------------------------------------------------------

    MFloatMatrix camera;

    if (useTextureVal && texturePlug.isConnected())
    {
        if (useUVVal)
        {
            MRenderUtil::sampleShadingNetwork(displaceTextureOutput,
                                              (int)vtxCount,
                                              false,
                                              false,
                                              camera,
                                              &points,
                                              &uCoords,
                                              &vCoords,
                                              &normals,
                                              &points,
                                              NULL,
                                              NULL,
                                              &filterSizes,
                                              colors,
                                              transparencies);
        }
        else
        {
            MRenderUtil::sampleShadingNetwork(displaceTextureOutput,
                                              (int)vtxCount,
                                              false,
                                              false,
                                              camera,
                                              &points,
                                              NULL,
                                              NULL,
                                              &normals,
                                              &points,
                                              NULL,
                                              NULL,
                                              &filterSizes,
                                              colors,
                                              transparencies);
        }
    }

    // Only continue if the color array has values. This is only a
    // safety measure as the colors array is empty while a shading
    // network is created.
    if (!colors.length())
        return status;

    // -----------------------------------------------------------------
    // calculate the position offset for all points
    // -----------------------------------------------------------------

    tbb::parallel_for(tbb::blocked_range<unsigned int>(0, vtxCount),
                      [&](tbb::blocked_range<unsigned int> r)
    {
        for (unsigned int k = r.begin(); k < r.end(); k ++)
        {
            MPoint point = meshPoints[k];

            float weight = weightValue(data, multiIndex, k);

            if (weight != 0)
            {
                // check if the transforms should be ignored
                if (!useTransformVal)
                    point *= mat;

                // define the offset for the points
                MFloatVector offsetVector;
                offsetVector.x = normals[k].x * offsetVal;
                offsetVector.y = normals[k].y * offsetVal;
                offsetVector.z = normals[k].z * offsetVal;

                // calculate the color strength from the texture
                MVector rgb;
                rgb = colors[k];

                if (averageColorsVal)
                {
                    rgb.z = rgb.y = rgb.x = (rgb.x + rgb.y + rgb.z) / 3;
                    normals[k].x *= (float)rgb.x;
                    normals[k].y *= (float)rgb.y;
                    normals[k].z *= (float)rgb.z;
                }
                else
                    normals[k] = rgb;

                float dropoff = dropoffs[k];
                // Calculate the dropoff based on the vertex color.
                // HasColors is a convenience switch which is only true
                // if vertex colors are present and dropoff should be
                // used.
                if (hasColors)
                {
                    float colorValue = getColorValue(colorList[k], colorMult);
                    dropoff = getMinDropoff(colorValue, dropoff);
                }

                // calculate the dropoffs
                for (unsigned l = 0; l < dropoffPositions.length(); l ++)
                {
                    MPoint pnt = meshPoints[k];

                    if (!localVal)
                        pnt *= mat;

                    float distValue = getDropoffDistance(point,
                                                         dropoffPositions[l],
                                                         inflUValues[l],
                                                         dropoff);
                    dropoff = getMinDropoff(distValue, dropoff);
                }

                MFloatVector result;
                result = scaleVal * env * weight * dropoff * normals[k];
                point += result + offsetVector;

                if (!useTransformVal)
                    point *= mat.inverse();
            }
            meshPoints[k] = point;
        }
    });

    outIter.setAllPositions(meshPoints);

    // Important for the EM evaluation.
    // Notify the EM that the mesh has been processed.
    data.setClean(outputGeomPlug);
    outputDataHandle.setMObject(outputDataHandle.asMesh());

    return status;
}

//
// Description:
//      Calculate a new color value with the given channel-based
//      multiplier and the gain and gamma values.
//
// Input Arguments:
//      color                   The current vertex color.
//      multiplier              The multiplier list.
//
// Return Value:
//      float                   The resulting color value.
//
float iDisplace::getColorValue(MColor color, int multiplier[4])
{
    float value = (color.r * multiplier[0]) +
                  (color.g * multiplier[1]) +
                  (color.b * multiplier[2]) +
                  (color.a * multiplier[3]);

    return pow(value * colorGainVal, colorGammaVal);
}

//
// Description:
//      Compare the given dropoff value with the given value and use the
//      smaller value.
//
// Input Arguments:
//      value                   The distance value.
//      dropoff                 The stored dropoff value.
//
// Return Value:
//      float                   The new dropoff value.
//
float iDisplace::getMinDropoff(float value, float dropoff)
{
    if (!invertVal)
    {
        if (dropoff < value)
            dropoff = value;
    }
    else
    {
        if (value > 1.0f)
            value = 1.0f;

        value = 1 - value;
        if (dropoff > value)
            dropoff = value;
    }
    return dropoff;
}

//
// Description:
//      Calculate the distance between the point and the dropoff
//      position.
//
// Input Arguments:
//      point                   The mesh point.
//      dropoffPoint            The dropoff point.
//      uValue                  The u value of the dropoff locator.
//      dropoffValue            The stored dropoff value.
//
// Return Value:
//      float                   The new dropoff value.
//
float iDisplace::getDropoffDistance(MPoint point,
                                    MPoint dropoffPoint,
                                    float uValue,
                                    float dropoffValue)
{
    MVector delta = dropoffPoint - point;
    double distance = delta.length();

    if (distance <= radiusVal)
    {
        // Get the amount of displacement based on the curve ramp.
        float distanceNormalized = (float)(distance / radiusVal);
        if (distanceNormalized > 1.0)
            distanceNormalized = 1.0;
        curveAttr.getValueAtPosition(distanceNormalized, dropoffValue);

        // Modify the displacement based on the curveFalloff ramp.
        float curveValue;
        curveAttrUVal.getValueAtPosition(uValue, curveValue);
        dropoffValue *= curveValue;
    }
    else if (invertVal)
        dropoffValue = 1.0f - dropoffValue;

    return dropoffValue;
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
