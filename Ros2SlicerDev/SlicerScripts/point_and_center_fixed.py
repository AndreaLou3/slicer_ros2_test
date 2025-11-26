import slicer

# -----------------------------------------------------------
# FUNCTION 1: Add a 3D point and center all slices on it
# -----------------------------------------------------------

def add_point_and_center_slices(x, y, z):
    # Create the main point node
    pointNode = slicer.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode", "MainPoint")
    pointNode.AddControlPoint([x, y, z])

    # Get slice nodes via layout manager (correct method)
    lm = slicer.app.layoutManager()
    red = lm.sliceWidget("Red").mrmlSliceNode()
    yellow = lm.sliceWidget("Yellow").mrmlSliceNode()
    green = lm.sliceWidget("Green").mrmlSliceNode()

    # Center each slice on the point
    for sliceNode in [red, yellow, green]:
        sliceNode.SetXYZOrigin(x, y, z)
        sliceNode.UpdateMatrices()

    return pointNode


# -----------------------------------------------------------
# FUNCTION 2: Axis-aligned projection (trivial and correct)
# -----------------------------------------------------------
# Since the default Slicer slices are axis aligned:
#   Red     (Sagittal) X fixed
#   Yellow  (Coronal)  Y fixed
#   Green   (Axial)    Z fixed
#
# The projection of a point onto each slice is simply:
#   proj = (x, y, z)
#
# These projected points lie EXACTLY on top of the original point.
# -----------------------------------------------------------

def create_axis_aligned_projections(pointNode):
    # Read original RAS coordinates
    x, y, z = pointNode.GetNthControlPointPositionWorld(0)

    # Create the projected points (all identical)
    projRed = slicer.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode", "Proj_Red")
    projRed.AddControlPoint([x, y, z])

    projYellow = slicer.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode", "Proj_Yellow")
    projYellow.AddControlPoint([x, y, z])

    projGreen = slicer.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode", "Proj_Green")
    projGreen.AddControlPoint([x, y, z])

    return projRed, projYellow, projGreen


# -----------------------------------------------------------
# MAIN EXECUTION
# -----------------------------------------------------------

# Example coordinate replace with your own
x, y, z = 30, 40, 50

mainPoint = add_point_and_center_slices(x, y, z)
projections = create_axis_aligned_projections(mainPoint)

print("Point and slice projections created successfully.")

