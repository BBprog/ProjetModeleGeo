#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QtMath>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "meshitem.h"
#include "courbebezier.h"
#include "discretisation.h"

namespace Ui {
class MainWindow;
}

enum DisplayMode {Normal, TemperatureMap, ColorShading, VertexColorShading};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // wrapper function to display all shapes and the original mesh (only if "show mesh" is checked)
    void display(MyMesh *_mesh);
    // display a single mesh
    void displayMesh(MyMesh *_mesh, DisplayMode mode = DisplayMode::Normal);
    // display a list of meshes
    void displayMeshes(QVector<MyMesh*> _meshes, DisplayMode mode = DisplayMode::Normal);

    void resetAllColorsAndThickness(MyMesh* _mesh);

private slots:

    // load a mesh
    void on_pushButton_chargement_clicked();
    // add a new shape mesh at the selected vertex position
    void on_pushButton_addShape_clicked();
    // show/hide original mesh
    void on_checkBox_showMesh_stateChanged(int arg1);

    // triggerred when a shape is selected in the list of shapes
    void onItemClicked(QListWidgetItem* item);

    // on shape's scale changed
    void on_spinBox_scale_valueChanged(double arg1);
    // on shape's angle changed
    void on_spinBox_angle_valueChanged(double arg1);
    // on shape's quality changed
    void on_spinBox_quality_valueChanged(int arg1);

    // keyboard events
    virtual void keyPressEvent( QKeyEvent* );

private:

    // original mesh
    MyMesh mesh;
    // shapes
    QHash<int, MeshItem*> items;
    // selected shape
    MeshItem *selectedItem;

    // selected vertex on the original mesh
    int vertexSelection;
    bool modevoisinage;

    Ui::MainWindow *ui;

    // select next vertex
    void incrVertexSelection();
    // select previous vertex
    void decrVertexSelection();
    // update the shape's tool panel
    void updateToolsUi(MeshItem* _item);
};

#endif // MAINWINDOW_H
