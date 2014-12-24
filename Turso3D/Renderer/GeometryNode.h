// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "../Graphics/GraphicsDefs.h"
#include "OctreeNode.h"

namespace Turso3D
{

class ConstantBuffer;
class IndexBuffer;
class Material;
class VertexBuffer;

/// Geometry types.
enum GeometryType
{
    GEOM_STATIC = 0,
    GEOM_INSTANCED
};

/// Description of geometry to be rendered. Scene nodes that render the same object can share these to reduce memory load and allow instancing.
struct TURSO3D_API Geometry : public RefCounted
{
    /// Construct with defaults.
    Geometry() :
        primitiveType(TRIANGLE_LIST),
        drawStart(0),
        drawCount(0)
    {
    }

    /// Geometry vertex buffer.
    SharedPtr<VertexBuffer> vertexBuffer;
    /// Geometry index buffer.
    SharedPtr<IndexBuffer> indexBuffer;
    /// Constant buffers.
    SharedPtr<ConstantBuffer> constantBuffers[MAX_SHADER_STAGES];
    /// Geometry's primitive type.
    PrimitiveType primitiveType;
    /// Draw range start. Specifies index start if index buffer defined, vertex start otherwise.
    size_t drawStart;
    /// Draw range count. Specifies number of indices if index buffer defined, number of vertices otherwise.
    size_t drawCount;
};

/// Base class for nodes that contain geometry to be rendered.
class TURSO3D_API GeometryNode : public OctreeNode
{
public:
    /// Construct.
    GeometryNode();
    /// Destruct.
    ~GeometryNode();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Called by Renderer.
    virtual void OnPrepareRender(Camera* camera);

    /// Set geometry type in all batches.
    void SetGeometryType(GeometryType type);
    /// Set number of geometries / batches.
    void SetNumGeometries(size_t num);
    /// Set geometry in batch.
    void SetGeometry(size_t index, Geometry* geometry);
    /// Set material in batch.
    void SetMaterial(size_t index, Material* material);
    /// Set local space bounding box.
    void SetBoundingBox(const BoundingBox& box);

    /// Return geometry type.
    GeometryType GetGeometryType() const { return geometryType; }
    /// Return number of geometries.
    size_t NumGeometries() const { return geometries.Size(); }
    /// Return geometry by batch index.
    Geometry* GetGeometry(size_t index) const;
    /// Return material by batch index.
    Material* GetMaterial(size_t index) const;
    /// Return all geometries.
    const Vector<SharedPtr<Geometry> >& Geometries() const { return geometries; }
    /// Return all materials.
    const Vector<SharedPtr<Material> >& Materials() const { return materials; }
    /// Return local space bounding box.
    const BoundingBox& GetBoundingBox() const { return boundingBox; }
    /// Return distance from camera in the current view.
    float Distance() const { return distance; }

protected:
    /// Recalculate the world bounding box.
    void OnWorldBoundingBoxUpdate() const override;

    /// Geometry type.
    GeometryType geometryType;
    /// Geometries in each batch.
    Vector<SharedPtr<Geometry> > geometries;
    /// Materials in each batch.
    Vector<SharedPtr<Material> > materials;
    /// Local space bounding box.
    BoundingBox boundingBox;
    /// Distance from camera in the current view.
    float distance;
};

}