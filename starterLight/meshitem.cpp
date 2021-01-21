#include "meshitem.h"
#include "materiel_courbures/courbures.h"


MeshItem::MeshItem(MyMesh _mesh, int _center, float _scale, float _angle, int _quality)
{
    this->meshBase = _mesh;
    this->center = _mesh.vertex_handle(_center);
    this->quality = _quality;
    this->angle = _angle;
    this->scale = _scale;

    generateMesh();
}

MeshItem::~MeshItem()
{
    delete mesh;
}

void MeshItem::generateMesh()
{
    // on créé la courbure avec le mesh original
    Courbures courb(meshBase);
    courb.normales_locales();

    // approximation quadratique
    MyQuad q = courb.fit_quad(center);

    // recupération des informations pour le changement de base
    MyMesh::Normal n = meshBase.normal(center) ;
    MyMesh::Point p = meshBase.point(center);

    Eigen::Vector3d ne(n[0], n[1], n[2]), Oz(0,0,1), axis;
    Eigen::Vector3d p_e(p[0], p[1], p[2]), pi_e ;

    axis = ne.cross(Oz) ;
    double sina = axis.norm(), cosa = ne.dot(Oz), angle ;
    if (sina >= 0)
        angle = acos(cosa) ;
    else
        angle = -acos(cosa) ;
    axis = axis.normalized() ;

    Eigen::AngleAxisd r(angle, axis) ;
    Eigen::Translation3d t(-p_e[0], -p_e[1], -p_e[2]) ;
    Eigen::Transform<double, 3, Eigen::Affine> ch_base = r * t ;

    // création de la courbe du motif en 2D (z = 0)
    QVector<Point*> c1_pts;
    c1_pts.append(new Point(0,0,0));
    c1_pts.append(new Point(1,1,0));
    c1_pts.append(new Point(0,-2,0));
    c1_pts.append(new Point(-1,1,0));
    c1_pts.append(new Point(0,0,0));
    CourbeBezier c1(c1_pts);

    // rotation de la courbe selon l'angle
    c1.rotate(this->angle);

    // discrétisation de la courbe selon la qualité
    Discretisation discret;
    discret.addCurve(c1);
    discret.setN(quality);
    discret.discr();

    QVector<Point> discr_pts = discret.getPoints();

    // création du mesh
    mesh = new MyMesh();

    // ajout des vertices
    for (int i = 0; i < discr_pts.size()-1; ++i) {
        Point pc = discr_pts.at(i);

        float x = pc.getX() * scale;
        float y = pc.getY() * scale;
        double z = q.quad_fun(x, y);

        MyMesh::Point p_n = MyMesh::Point(x, y, z);

        pi_e << p_n[0], p_n[1], p_n[2];
        pi_e = ch_base.inverse() * pi_e;

        mesh->add_vertex(MyMesh::Point(pi_e[0], pi_e[1], pi_e[2]));
    }

    // création des arrêtes
    VertexHandle v_a = mesh->vertex_handle(0);
    VertexHandle v_b;
    for (int i = 1; i < mesh->n_vertices(); ++i) {
        v_b = mesh->vertex_handle(i);
        mesh->new_edge(v_a, v_b);
        v_a = v_b;
    }
}

void MeshItem::select()
{
    for (MyMesh::VertexIter curVert = mesh->vertices_begin(); curVert != mesh->vertices_end(); curVert++)
    {
        mesh->data(*curVert).thickness = 4;
        mesh->set_color(*curVert, MyMesh::Color(0, 255, 0));
    }
    for (MyMesh::EdgeIter curEdge = mesh->edges_begin(); curEdge != mesh->edges_end(); curEdge++)
    {
        mesh->data(*curEdge).thickness = 4;
        mesh->set_color(*curEdge, MyMesh::Color(0, 255, 0));
    }
}

void MeshItem::unSelect()
{
    for (MyMesh::VertexIter curVert = mesh->vertices_begin(); curVert != mesh->vertices_end(); curVert++)
    {
        mesh->data(*curVert).thickness = 4;
        mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }
    for (MyMesh::EdgeIter curEdge = mesh->edges_begin(); curEdge != mesh->edges_end(); curEdge++)
    {
        mesh->data(*curEdge).thickness = 4;
        mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

void MeshItem::setQuality(int _quality)
{
    this->quality = _quality;
    generateMesh();
}

void MeshItem::setAngle(float _angle)
{
    this->angle = _angle;
    generateMesh();
}

void MeshItem::setScale(float _scale)
{
    this->scale = _scale;
    generateMesh();
}

float MeshItem::getScale()
{
    return scale;
}

float MeshItem::getAngle()
{
    return angle;
}

int MeshItem::getQuality()
{
    return quality;
}

MyMesh* MeshItem::getMesh()
{
    return mesh;
}
