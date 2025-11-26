import slicer
import vtk

# -----------------------------
# Function 1: Place point & center slices
# -----------------------------
def place_point_and_center_slices(x, y, z, name="OriginalPoint"):
    """
    Adds a 3D fiducial point at (x, y, z) and centers
    the Red, Yellow, and Green slice views on it.
    Returns the fiducial node.
    """
    # Add the original point
    fidNode = slicer.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode")
    fidNode.SetName(name)
    fidNode.AddControlPoint([x, y, z])

    # Get live slice nodes via layout manager
    redSlice = slicer.app.layoutManager().sliceWidget("Red").mrmlSliceNode()
    yellowSlice = slicer.app.layoutManager().sliceWidget("Yellow").mrmlSliceNode()
    greenSlice = slicer.app.layoutManager().sliceWidget("Green").mrmlSliceNode()

    # Center slices on the point
    redSlice.SetSliceOffset(x)     # sagittal
    yellowSlice.SetSliceOffset(y)  # coronal
    greenSlice.SetSliceOffset(z)   # axial

    # Refresh viewers
    slicer.app.processEvents()

    return fidNode

# -----------------------------
# Function 2: Project point onto slices
# -----------------------------
def project_point_to_slices(point3D):
    """
    Projects a 3D point onto the Red, Yellow, and Green slices
    and adds a fiducial node for each projected point.
    Returns a dictionary of projected nodes.
    """

    def project_point_to_slice(point3D, sliceNode):
        """Helper to project a point onto a slice plane."""

        # Get slice-to-RAS matrix (vtkMatrix4x4)
        sliceToRAS = sliceNode.GetSliceToRAS()

        # Invert to get RAS-to-slice
        rasToSlice = vtk.vtkMatrix4x4()
        vtk.vtkMatrix4x4.Invert(sliceToRAS, rasToSlice)

        # Convert point to slice coordinates (homogeneous)
        pointSlice = [point3D[0], point3D[1], point3D[2], 1.0]
        projectedSlice = [0, 0, 0, 0]
        rasToSlice.MultiplyPoint(pointSlice, projectedSlice)

        # Zero Z to project onto slice plane
        projectedSlice[2] = 0.0

        # Transform back to RAS/world coordinates
        projectedWorld = [0, 0, 0, 0]
        sliceToRAS.MultiplyPoint(projectedSlice, projectedWorld)

        return projectedWorld[:3]

    # Get live slice nodes
    sliceNodes = {
        "Red": slicer.app.layoutManager().sliceWidget("Red").mrmlSliceNode(),
        "Yellow": slicer.app.layoutManager().sliceWidget("Yellow").mrmlSliceNode(),
        "Green": slicer.app.layoutManager().sliceWidget("Green").mrmlSliceNode(),
    }

    projectedNodes = {}
    for name, sliceNode in sliceNodes.items():
        projPoint = project_point_to_slice(point3D, sliceNode)
        node = slicer.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode")
        node.SetName(f"Proj_{name}")
        node.AddControlPoint(projPoint)
        projectedNodes[name] = node

    # Refresh viewers
    slicer.app.processEvents()
    return projectedNodes

# -----------------------------
# Main: Call both functions
# -----------------------------
x, y, z = 10, 20, 30  # Replace with your coordinates

# 1. Place original point and center slices
originalNode = place_point_and_center_slices(x, y, z)

# 2. Project point onto slice planes
projectedNodes = project_point_to_slices([x, y, z])

# Print results
print("Original point node:", originalNode.GetName())
for name, node in projectedNodes.items():
    print(f"Projected node on {name} slice:", node.GetName())

