#ifndef MESHITEM_H
#define MESHITEM_H

#include <QDebug>
#include <QListWidgetItem>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );
    EdgeAttributes( OpenMesh::Attributes::Color );
    // vertex thickness
    VertexTraits{float thickness; float value; Color faceShadingColor;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;


/*
 * Représente un motif
 * Hérite de QListWidgetItem pour pouvoir le placer dans un QListWidget *
 */
class MeshItem : public QListWidgetItem
{
public:

    // constructeur
    MeshItem(MyMesh _mesh, int _center, float _scale = 1, float _angle = 0, int _quality = 50);
    ~MeshItem();

    // met le motif en surbrillance
    void select();
    // colorise le motif par default
    void unSelect();

    // accesseurs
    void setQuality(int _quality);
    void setAngle(float _angle);
    void setScale(float _scale);
    float getScale();
    float getAngle();
    int getQuality();
    MyMesh* getMesh();

private:

    // mesh original
    MyMesh meshBase;
    // mesh du motif
    MyMesh *mesh;
    // centre du motif
    VertexHandle center;

    // échelle
    float scale;
    // rotation
    float angle;
    // nombre de points discrétisés
    int quality;

    // génere le mesh du motif
    void generateMesh();
};

#endif // MESHITEM_H
