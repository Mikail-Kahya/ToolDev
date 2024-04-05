#include "minecraftform.h"
#include "./ui_minecraftform.h"

#include <qfiledialog.h>
#include <qstandardpaths.h>

#include "MinecraftParser.h"

MinecraftForm::MinecraftForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MinecraftForm)
{
    ui->setupUi(this);
    ui->list->setColumnWidth(static_cast<int>(Field::destroy), 50);
}

MinecraftForm::~MinecraftForm()
{
    delete ui;
}

void MinecraftForm::on_submit_clicked()
{
    Block block{};
    // Set form data
    block.position.setX(ui->xInput->value());
    block.position.setY(ui->yInput->value());
    block.position.setZ(ui->zInput->value());
    block.material = ui->materialInput->currentText();
    AddBlock(block);
}

QString MinecraftForm::GetVec3Format(const QVector3D &vec) const
{
    return
    {   "[ " +
        QString::number(vec.x()) +
        ", " +
        QString::number(vec.y()) +
        ", " +
        QString::number(vec.z()) +
        " ]"
    };
}

void MinecraftForm::on_list_cellClicked(int row, int column)
{
    if (column != static_cast<int>(Field::destroy))
        return;

    ui->list->removeRow(row);
    m_Blocks.erase(m_Blocks.begin() + row);
}

void MinecraftForm::on_actionLoad_triggered()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Json files"),
                                                          QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                          tr("Json files (*.json)"));

    if (fileName.isEmpty())
        return;

    m_Blocks.clear();
    ui->list->clear();
    ui->list->setRowCount(0);

    MinecraftParser parser{};
    parser.ParseJson(fileName.toStdString());
    for (const auto& layer : parser.GetLayers())
    {
        const std::string& material{ layer.second.material.name };
        for (const Cube& cube : layer.second.cubes)
        {
            const Vector3& position{ cube.GetPosition() };
            Block block{};
            block.position.setX(position.x);
            block.position.setY(position.y);
            block.position.setZ(position.z);
            block.material = material.c_str();

            AddBlock(block);
        }
    }
}

void MinecraftForm::on_actionSaveAsJson_triggered()
{
    const QString fileName = QFileDialog::getSaveFileName(this,
                                                          tr("Json files"),
                                                          QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                          tr("Json files (*.json)"));

    MinecraftParser parser{};
    for (const Block& block : m_Blocks)
        parser.AddCube(Vector3{ block.position.x(), block.position.y(), block.position.z() }, block.material.toStdString());

    parser.SaveJson(fileName.toStdString());
}

void MinecraftForm::on_actionSaveAsObj_triggered()
{
    const QString fileName = QFileDialog::getSaveFileName(this,
                                                          tr("Open Wavefront obj"),
                                                          QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                          tr("Obj Files (*.obj)"));

    MinecraftParser parser{ static_cast<float>(ui->blockSizeInput->value()) };
    for (const Block& block : m_Blocks)
        parser.AddCube(Vector3{ block.position.x(), block.position.y(), block.position.z() }, block.material.toStdString());

    parser.SaveObj(fileName.toStdString());
}

void MinecraftForm::AddBlock(const Block& block)
{
    m_Blocks.emplace_back(block);

    // Insert new row
    const int row{ ui->list->rowCount() };
    ui->list->insertRow(row);
    ui->list->setItem(row, static_cast<int>(Field::position), new QTableWidgetItem(GetVec3Format(block.position)));
    ui->list->setItem(row, static_cast<int>(Field::material), new QTableWidgetItem(block.material));
    ui->list->setItem(row, static_cast<int>(Field::destroy), new QTableWidgetItem("delete"));
}
