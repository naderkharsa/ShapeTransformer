#include "shapetransformer.h"
#include "ui_shapetransformer.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

ShapeTransformer::ShapeTransformer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShapeTransformer)
    , scene(new QGraphicsScene(this)) // Initialize the QGraphicsScene
{
    ui->setupUi(this);

    // Set up the graphics view with the scene
    ui->graphicsView->setScene(scene);

    // Make sure the scene size is exactly the size of the QGraphicsView
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    // Connect buttons to the functions for drawing shapes and actions
    connect(ui->btnCircle, &QPushButton::clicked, this, &ShapeTransformer::drawCircle);
    connect(ui->btnRectangle, &QPushButton::clicked, this, &ShapeTransformer::drawRectangle);
    connect(ui->btnTransform, &QPushButton::clicked, this, &ShapeTransformer::applyTransformation);
    connect(ui->btnDelete, &QPushButton::clicked, this, &ShapeTransformer::deleteItems);
    connect(ui->scaleSlider, &QSlider::valueChanged, this, &ShapeTransformer::on_scaleSlider_valueChanged);

}

QColor ShapeTransformer::getSelectedColor() {
    // Get the selected color from the ComboBox
    QString selectedColor = ui->colorComboBox->currentText();

    if (selectedColor == "Red") {
        return Qt::red;
    } else if (selectedColor == "Black") {
        return Qt::black;
    } else if (selectedColor == "Blue") {
        return Qt::blue;
    } else if (selectedColor == "Green") {
        return Qt::green;
    } else if (selectedColor == "White") {
        return Qt::white;
    } else {
        return Qt::black;  // Default color in case no valid color is selected
    }
}

ShapeTransformer::~ShapeTransformer()
{
    delete ui;
}

void ShapeTransformer::mousePressEvent(QMouseEvent *event) {
    QPointF viewPos = event->pos();  // This is in QGraphicsView coordinates
    QPointF scenePos = ui->graphicsView->mapToScene(viewPos.toPoint());  // Map to the scene

    // Debugging: Print the coordinates we are mapping
    qDebug() << "View coordinates: " << viewPos;
    qDebug() << "Mapped Scene coordinates: " << scenePos;

    // Check if there is a shape at this position
    QList<QGraphicsItem*> itemsAtPosition = scene->items(scenePos);

    // Safely remove the previous green dot if it exists and is still in the scene
    if (currentDot && scene->items().contains(currentDot)) {
        qDebug() << "Removing current green dot";
        scene->removeItem(currentDot);  // Remove it from the scene
        currentDot = nullptr;  // Clear the pointer to prevent invalid access
    }

    // If there is a previously selected item, reset its border
    if (selectedItem) {
        qDebug() << "Resetting the border of the previously selected item";
        if (auto *rect = qgraphicsitem_cast<QGraphicsRectItem *>(selectedItem)) {
            rect->setPen(QPen(Qt::black));  // Reset the border to black
        } else if (auto *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem *>(selectedItem)) {
            ellipse->setPen(QPen(Qt::black));  // Reset the border to black
        }
        selectedItem = nullptr;  // Clear the selection
    }

    if (!itemsAtPosition.isEmpty()) {
        // Select the first item found and highlight it with a thick yellow outline
        qDebug() << "Item found at this position, selecting it.";
        selectedItem = itemsAtPosition.first();

        if (auto *rect = qgraphicsitem_cast<QGraphicsRectItem *>(selectedItem)) {
            rect->setPen(QPen(Qt::yellow, 5));  // Set a thick yellow border with width 5
        } else if (auto *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem *>(selectedItem)) {
            ellipse->setPen(QPen(Qt::yellow, 5));  // Set a thick yellow border with width 5
        }

        selectedPosition = QPointF();  // Clear the position for drawing a new shape
    } else {
        // No item at this position, so mark the position to draw a new shape
        qDebug() << "No item found at this position, creating a new dot.";
        selectedPosition = scenePos;

        // Mark the clicked position with a small green ellipse and store its reference
        currentDot = scene->addEllipse(scenePos.x() - 2, scenePos.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::green));
    }
}

// Slot for drawing a circle
void ShapeTransformer::drawCircle() {
    if (!selectedPosition.isNull()) {
        // Get the selected color
        QColor color = getSelectedColor();

        // Draw the circle with the selected color
        scene->addEllipse(selectedPosition.x() - 25, selectedPosition.y() - 25, 50, 50,
                          QPen(color), QBrush(color));
    }
}

// Slot for drawing a rectangle
void ShapeTransformer::drawRectangle() {
    if (!selectedPosition.isNull()) {
        // Get the selected color
        QColor color = getSelectedColor();

        // Draw the rectangle with the selected color
        scene->addRect(selectedPosition.x() - 25, selectedPosition.y() - 25, 50, 50,
                       QPen(color), QBrush(color));
    }
}

// Slot for applying transformations
void ShapeTransformer::applyTransformation() {
    if (selectedItem) {
        // Set the origin of the transformation to the center of the item
        QRectF itemBounds = selectedItem->boundingRect();  // Get the item's bounds
        selectedItem->setTransformOriginPoint(itemBounds.center());  // Set origin to center of the item

        // Rotate the item by 15 degrees each time Transform is clicked
        qreal currentRotation = selectedItem->rotation();
        selectedItem->setRotation(currentRotation + 15);  // Add 15 degrees to the current rotation
    }
}

// Slot for scaling value change
void ShapeTransformer::on_scaleSlider_valueChanged(int value) {
    if (selectedItem) {
        double scaleFactor = value / 50.0;  // Assuming 50 is the base size

        if (auto *rect = qgraphicsitem_cast<QGraphicsRectItem *>(selectedItem)) {
            // Scaling a rectangle
            rect->setTransformOriginPoint(rect->boundingRect().center());  // Set the origin for scaling
            rect->setScale(scaleFactor);
        } else if (auto *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem *>(selectedItem)) {
            // Scaling a circle
            ellipse->setTransformOriginPoint(ellipse->boundingRect().center());  // Set the origin for scaling
            ellipse->setScale(scaleFactor);
        }
    }
}


// Slot for deleting items from the scene
void ShapeTransformer::deleteItems()
{
    scene->clear();
}
