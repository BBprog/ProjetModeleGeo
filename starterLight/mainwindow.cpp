#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materiel_courbures/courbures.h"


void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if(vz == 0)
        vz = .0001;

    float v = sqrt( vx*vx + vy*vy + vz*vz );
    float ax = 57.2957795*acos( vz/v );
    if ( vz < 0.0 )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef( x1,y1,z1 );
    glRotatef(ax, rx, ry, 0.0);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, subdivisions, 1);

    //draw the first cap
    gluQuadricOrientation(quadric,GLU_INSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glTranslatef( 0,0,v );

    //draw the second cap
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();
}

void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions) {
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
    gluDeleteQuadric(quadric);
}

/* **** début de la partie boutons et IHM **** */

void MainWindow::on_pushButton_chargement_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData());

    mesh.update_normals();
    mesh.request_vertex_colors() ;

    vertexSelection = 0;

    // suppression des éventuels motifs
    items.clear();
    ui->listWidget_shapes->clear();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);

    // on affiche le maillage
    displayMesh(&mesh);
}

void MainWindow::on_pushButton_addShape_clicked()
{
    // creation du motif et attribution d'une id
    MeshItem *item = new MeshItem(mesh, vertexSelection, 0.1);
    int id = items.size();
    item->setData(0, id);

    // selectionne le motif pour le mettre en surbrillance
    item->select();

    // ajoute le motif à la liste
    ui->listWidget_shapes->addItem(item);
    items.insert(id, item);

    display(&mesh);
}

/* **** fin de la partie boutons et IHM **** */



/* **** fonctions supplémentaires **** */

// permet d'initialiser les couleurs et les épaisseurs des élements du maillage
void MainWindow::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }

    if (vertexSelection >= 0) {
        VertexHandle vh = _mesh->vertex_handle(vertexSelection);
        _mesh->set_color(vh, MyMesh::Color(0, 255, 0));
        _mesh->data(vh).thickness = 5;
    }
}

// charge un objet MyMesh dans l'environnement OpenGL
void MainWindow::displayMesh(MyMesh* _mesh, DisplayMode mode)
{
    GLuint* triIndiceArray = new GLuint[_mesh->n_faces() * 3];
    GLfloat* triCols = new GLfloat[_mesh->n_faces() * 3 * 3];
    GLfloat* triVerts = new GLfloat[_mesh->n_faces() * 3 * 3];

    int i = 0;

    if(mode == DisplayMode::TemperatureMap)
    {
        QVector<float> values;
        for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
            values.append(fabs(_mesh->data(*curVert).value));
        qSort(values);

        float range = values.at(values.size()*0.8);

        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;

        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    if(mode == DisplayMode::Normal)
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    if(mode == DisplayMode::ColorShading)
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->data(*fvIt).faceShadingColor[0]; triCols[3*i+1] = _mesh->data(*fvIt).faceShadingColor[1]; triCols[3*i+2] = _mesh->data(*fvIt).faceShadingColor[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->data(*fvIt).faceShadingColor[0]; triCols[3*i+1] = _mesh->data(*fvIt).faceShadingColor[1]; triCols[3*i+2] = _mesh->data(*fvIt).faceShadingColor[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->data(*fvIt).faceShadingColor[0]; triCols[3*i+1] = _mesh->data(*fvIt).faceShadingColor[1]; triCols[3*i+2] = _mesh->data(*fvIt).faceShadingColor[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    if (mode == DisplayMode::VertexColorShading)
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fvIt)[0]; triCols[3*i+1] = _mesh->color(*fvIt)[1]; triCols[3*i+2] = _mesh->color(*fvIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fvIt)[0]; triCols[3*i+1] = _mesh->color(*fvIt)[1]; triCols[3*i+2] = _mesh->color(*fvIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fvIt)[0]; triCols[3*i+1] = _mesh->color(*fvIt)[1]; triCols[3*i+2] = _mesh->color(*fvIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;

    GLuint* linesIndiceArray = new GLuint[_mesh->n_edges() * 2];
    GLfloat* linesCols = new GLfloat[_mesh->n_edges() * 2 * 3];
    GLfloat* linesVerts = new GLfloat[_mesh->n_edges() * 2 * 3];

    i = 0;
    QHash<float, QList<int> > edgesIDbyThickness;
    for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
    {
        float t = _mesh->data(*eit).thickness;
        if(t > 0)
        {
            if(!edgesIDbyThickness.contains(t))
                edgesIDbyThickness[t] = QList<int>();
            edgesIDbyThickness[t].append((*eit).idx());
        }
    }
    QHashIterator<float, QList<int> > it(edgesIDbyThickness);
    QList<QPair<float, int> > edgeSizes;
    while (it.hasNext())
    {
        it.next();

        for(int e = 0; e < it.value().size(); e++)
        {
            int eidx = it.value().at(e);

            MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh1)[0];
            linesVerts[3*i+1] = _mesh->point(vh1)[1];
            linesVerts[3*i+2] = _mesh->point(vh1)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;

            MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh2)[0];
            linesVerts[3*i+1] = _mesh->point(vh2)[1];
            linesVerts[3*i+2] = _mesh->point(vh2)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;
        }
        edgeSizes.append(qMakePair(it.key(), it.value().size()));
    }

    ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);

    delete[] linesIndiceArray;
    delete[] linesCols;
    delete[] linesVerts;

    GLuint* pointsIndiceArray = new GLuint[_mesh->n_vertices()];
    GLfloat* pointsCols = new GLfloat[_mesh->n_vertices() * 3];
    GLfloat* pointsVerts = new GLfloat[_mesh->n_vertices() * 3];

    i = 0;
    QHash<float, QList<int> > vertsIDbyThickness;
    for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
    {
        float t = _mesh->data(*vit).thickness;
        if(t > 0)
        {
            if(!vertsIDbyThickness.contains(t))
                vertsIDbyThickness[t] = QList<int>();
            vertsIDbyThickness[t].append((*vit).idx());
        }
    }
    QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);
    QList<QPair<float, int> > vertsSizes;

    while (vitt.hasNext())
    {
        vitt.next();

        for(int v = 0; v < vitt.value().size(); v++)
        {
            int vidx = vitt.value().at(v);

            pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
            pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
            pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
            pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
            pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
            pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
            pointsIndiceArray[i] = i;
            i++;
        }
        vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
    }

    ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}
void MainWindow::displayMeshes(QVector<MyMesh*> _meshes, DisplayMode mode)
{
    int nbFace = 0;
    for (MyMesh *_mesh : _meshes) {
        nbFace += _mesh->n_faces();
    }

    GLuint* triIndiceArray = new GLuint[nbFace * 3];
    GLfloat* triCols = new GLfloat[nbFace * 3 * 3];
    GLfloat* triVerts = new GLfloat[nbFace * 3 * 3];

    int i = 0;

    for (MyMesh *_mesh : _meshes) {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    ui->displayWidget->loadMesh(triVerts, triCols, nbFace * 3 * 3, triIndiceArray, nbFace * 3);

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;


    int nbEdges = 0;
    for (MyMesh *_mesh : _meshes) {
        nbEdges += _mesh->n_edges();
    }

    GLuint* linesIndiceArray = new GLuint[nbEdges * 2];
    GLfloat* linesCols = new GLfloat[nbEdges * 2 * 3];
    GLfloat* linesVerts = new GLfloat[nbEdges * 2 * 3];

    i = 0;
    QList<QPair<float, int> > edgeSizes;
    for (MyMesh *_mesh : _meshes) {
        QHash<float, QList<int> > edgesIDbyThickness;
        for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
        {
            float t = _mesh->data(*eit).thickness;
            if(t > 0)
            {
                if(!edgesIDbyThickness.contains(t))
                    edgesIDbyThickness[t] = QList<int>();
                edgesIDbyThickness[t].append((*eit).idx());
            }
        }
        QHashIterator<float, QList<int> > it(edgesIDbyThickness);

        while (it.hasNext())
        {
            it.next();

            for(int e = 0; e < it.value().size(); e++)
            {
                int eidx = it.value().at(e);

                MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
                linesVerts[3*i+0] = _mesh->point(vh1)[0];
                linesVerts[3*i+1] = _mesh->point(vh1)[1];
                linesVerts[3*i+2] = _mesh->point(vh1)[2];
                linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
                linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
                linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
                linesIndiceArray[i] = i;
                i++;

                MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
                linesVerts[3*i+0] = _mesh->point(vh2)[0];
                linesVerts[3*i+1] = _mesh->point(vh2)[1];
                linesVerts[3*i+2] = _mesh->point(vh2)[2];
                linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
                linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
                linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
                linesIndiceArray[i] = i;
                i++;
            }
            edgeSizes.append(qMakePair(it.key(), it.value().size()));
        }
    }

    ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);


    int nbVertices = 0;
    for (MyMesh *_mesh : _meshes) {
        nbVertices += _mesh->n_vertices();
    }

    GLuint* pointsIndiceArray = new GLuint[nbVertices];
    GLfloat* pointsCols = new GLfloat[nbVertices * 3];
    GLfloat* pointsVerts = new GLfloat[nbVertices * 3];

    i = 0;
    QList<QPair<float, int> > vertsSizes;
    for (MyMesh *_mesh : _meshes) {
        QHash<float, QList<int> > vertsIDbyThickness;
        for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
        {
            float t = _mesh->data(*vit).thickness;
            if(t > 0)
            {
                if(!vertsIDbyThickness.contains(t))
                    vertsIDbyThickness[t] = QList<int>();
                vertsIDbyThickness[t].append((*vit).idx());
            }
        }
        QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);

        while (vitt.hasNext())
        {
            vitt.next();

            for(int v = 0; v < vitt.value().size(); v++)
            {
                int vidx = vitt.value().at(v);

                pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
                pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
                pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
                pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
                pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
                pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
                pointsIndiceArray[i] = i;
                i++;
            }
            vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
        }
    }

    ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}
void MainWindow::display(MyMesh* _mesh)
{
    QVector<MyMesh*> mm;
    if (ui->checkBox_showMesh->isChecked()) {
        mm.push_back(_mesh);
    }
    for (MeshItem *item : items.values()) {
        mm.push_back(item->getMesh());
    }
    displayMeshes(mm);
}

void MainWindow::incrVertexSelection() {
    if (vertexSelection == mesh.n_vertices()) {
        vertexSelection = 0;
    }
    else {
        vertexSelection++;
    }
}
void MainWindow::decrVertexSelection() {
    if (vertexSelection == 0) {
        vertexSelection = mesh.n_vertices();
    }
    else {
        vertexSelection--;
    }
}

void MainWindow::updateToolsUi(MeshItem* _item)
{
    ui->tools->setEnabled(true);
    ui->spinBox_scale->setValue(_item->getScale());
    ui->spinBox_quality->setValue(_item->getQuality());
    ui->spinBox_angle->setValue(_item->getAngle());
}

void MainWindow::onItemClicked(QListWidgetItem *item)
{
    // recupération du motif selectionné
    this->selectedItem = items[item->data(0).toInt()];

    // deselectionne les autres motifs
    for (MeshItem *item : items.values()) {
        item->unSelect();
    }
    selectedItem->select();

    // met à jour le panel d'option en fonction du motif
    updateToolsUi(selectedItem);

    display(&mesh);
}

void MainWindow::on_spinBox_scale_valueChanged(double arg1)
{
    selectedItem->setScale(arg1);
    selectedItem->select();

    display(&mesh);
}

void MainWindow::on_spinBox_angle_valueChanged(double arg1)
{
    selectedItem->setAngle(arg1);
    selectedItem->select();

    display(&mesh);
}

void MainWindow::on_spinBox_quality_valueChanged(int arg1)
{
    selectedItem->setQuality(arg1);
    selectedItem->select();

    display(&mesh);
}

void MainWindow::on_checkBox_showMesh_stateChanged(int arg1)
{
    display(&mesh);
}


void MainWindow::keyPressEvent(QKeyEvent* _event)
{
    // RIGHT_ARROW : selectionne le vertex suivant
    if (Qt::Key_Right == _event->key()) {
        incrVertexSelection();
        resetAllColorsAndThickness(&mesh);
        display(&mesh);
    }
    // LEFT_ARROW : selectionne le vertex précédent
    else if (Qt::Key_Left == _event->key()) {
        decrVertexSelection();
        resetAllColorsAndThickness(&mesh);
        display(&mesh);
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    vertexSelection = 0;
    modevoisinage = false;

    ui->setupUi(this);

    connect(ui->listWidget_shapes, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}
