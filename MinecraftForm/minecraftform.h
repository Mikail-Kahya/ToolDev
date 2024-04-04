#ifndef MINECRAFTFORM_H
#define MINECRAFTFORM_H

#include <QMainWindow>
#include <QVector3D>

QT_BEGIN_NAMESPACE
namespace Ui {
class MinecraftForm;
}

class QTableWidgetItem;
QT_END_NAMESPACE

class MinecraftForm : public QMainWindow
{
    Q_OBJECT

public:
    MinecraftForm(QWidget *parent = nullptr);
    ~MinecraftForm();

private slots:
    void on_submit_clicked();
    void on_list_cellClicked(int row, int column);
    void on_actionLoad_triggered();
    void on_actionSaveAsJson_triggered();
    void on_actionSaveAsObj_triggered();

private:
    enum class Field
    {
        position,
        material,
        destroy
    };

    struct Block
    {
        QVector3D position{};
        QString material{};
    };

    void AddBlock(const Block& block);
    QString GetVec3Format(const QVector3D &vec) const;

    Ui::MinecraftForm *ui;
    std::vector<Block> m_Blocks{};
};
#endif // MINECRAFTFORM_H
