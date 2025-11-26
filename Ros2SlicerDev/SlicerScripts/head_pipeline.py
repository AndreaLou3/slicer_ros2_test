import slicer
from slicer.util import getNode

# ----------------------------
# CONFIGURATION
# ----------------------------
VOLUME_NAME = "patient-head.dcm"  # Change this if your volume node has a different name
SLICE_OFFSET_VALUE = 0.0          # Example: set all slice offsets to 0.0 mm

# ----------------------------
# STEP 1: Get the loaded volume node
# ----------------------------
volume_node = getNode(VOLUME_NAME)
if volume_node is None:
    raise RuntimeError(f"No volume named '{VOLUME_NAME}' found. Make sure you imported the DICOM series.")

print(f"Found volume: {volume_node.GetName()}")

# ----------------------------
# STEP 2: Set slice offsets for all views
# ----------------------------
slice_nodes = slicer.util.getNodesByClass("vtkMRMLSliceNode")
for sn in slice_nodes:
    sn.SetSliceOffset(SLICE_OFFSET_VALUE)
print(f"Set slice offsets for all views to {SLICE_OFFSET_VALUE}")

# ----------------------------
# STEP 3: Show slices and enable intersections
# ----------------------------
for sn in slice_nodes:
    sn.SetSliceVisible(True)
    composite_node_id = sn.GetSliceCompositeNodeID()
    composite_node = slicer.mrmlScene.GetNodeByID(composite_node_id)
    if composite_node:
        composite_node.SetSliceIntersectionVisibility(True)

# ----------------------------
# STEP 4: Enable volume rendering if not already present
# ----------------------------
if not volume_node.GetDisplayNode():
    slicer.modules.volumerendering.logic().CreateDefaultVolumeRenderingNodes(volume_node)

# Center the 3D view on the volume
view_3d = slicer.app.layoutManager().threeDWidget(0).threeDView()
view_3d.resetFocalPoint()

print("3D view with intersecting planes enabled.")
