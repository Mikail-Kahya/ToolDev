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
    void on_actionHideList_triggered(bool checked);
    void on_actionOpen_triggered(void);
    void on_actionHideForm_triggered(bool checked);
    void on_submit_clicked();
    void on_list_cellClicked(int row, int column);
    void on_actionSaveAsJson_triggered();

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

    QString GetVec3Format(const QVector3D &vec) const;

    Ui::MinecraftForm *ui;
    std::vector<Block> m_Blocks{};
};
#endif // MINECRAFTFORM_H
