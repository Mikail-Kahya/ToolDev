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
    on_actionHideForm_triggered(ui->actionHideForm->isChecked());
    on_actionHideList_triggered(ui->actionHideList->isChecked());
}

MinecraftForm::~MinecraftForm()
{
    delete ui;
}

void MinecraftForm::on_actionOpen_triggered()
{
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open Wavefront obj"),
                                                          QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                          tr("Obj Files (*.obj)"));

    QFile file{ fileName };

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
}

void MinecraftForm::on_actionHideForm_triggered(bool checked)
{
    ui->formWrapper->setVisible(!checked);
}

void MinecraftForm::on_submit_clicked()
{
    Block block{};

    // Set form data
    block.position.setX(ui->xInput->value());
    block.position.setY(ui->yInput->value());
    block.position.setZ(ui->zInput->value());
    block.material = ui->materialInput->currentText();
    m_Blocks.emplace_back(block);

    // Insert new row
    const int row{ ui->list->rowCount() };
    ui->list->insertRow(row);
    ui->list->setItem(row, static_cast<int>(Field::position), new QTableWidgetItem(GetVec3Format(block.position)));
    ui->list->setItem(row, static_cast<int>(Field::material), new QTableWidgetItem(block.material));
    ui->list->setItem(row, static_cast<int>(Field::destroy), new QTableWidgetItem("delete"));
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

void MinecraftForm::on_actionHideList_triggered(bool checked)
{
    ui->listWrapper->setVisible(!checked);
}

void MinecraftForm::on_list_cellClicked(int row, int column)
{
    if (column != static_cast<int>(Field::destroy))
        return;

    ui->list->removeRow(row);
    m_Blocks.erase(m_Blocks.begin() + row);
}

void MinecraftForm::on_actionSaveAsJson_triggered()
{
    const QString fileName = QFileDialog::getSaveFileName(this);
    //QFile file{ fileName };
    //file.open(QIODevice::WriteOnly);
    //if (!file.isOpen())
    //    return;

    MinecraftParser parser{};
    for (const Block& block : m_Blocks)
        parser.AddCube(Vector3{ block.position.x(), block.position.y(), block.position.z() }, block.material.toStdString());

    parser.SaveJson(fileName.toStdString());

    //file.close();
}

