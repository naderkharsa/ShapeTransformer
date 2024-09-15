#ifndef SHAPETRANSFORMER_H
#define SHAPETRANSFORMER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class ShapeTransformer; }
QT_END_NAMESPACE

class ShapeTransformer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShapeTransformer(QWidget *parent = nullptr);
    ~ShapeTransformer();

protected:
    // Capture mouse press event in the graphics view
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::ShapeTransformer *ui;
    QGraphicsScene *scene;
    QGraphicsItem *selectedItem = nullptr;  // Add this to the private section
    QColor getSelectedColor();  // Function to get the selected color
    QGraphicsEllipseItem* currentDot;
    // Store the last clicked position
    QPointF selectedPosition;

private slots:
    // Slot functions for buttons
    void drawCircle();
    void drawRectangle();
    void applyTransformation();
    void deleteItems();
    void on_scaleSlider_valueChanged(int value);  // Slot to scale the selected item when the slider value changes
};

#endif // SHAPETRANSFORMER_H
