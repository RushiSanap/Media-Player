#ifndef ADVDIALOG_H
#define ADVDIALOG_H

#include <QDialog>
#include <QVideoWidget>
#include <QSlider>

namespace Ui {
class AdvDialog;
}

class AdvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvDialog(QWidget *parent = nullptr, QVideoWidget *vw = nullptr);
    ~AdvDialog();

private slots:

    void on_HueSlider_sliderMoved(int position);

    void on_ContrastSlider_sliderMoved(int position);

    void on_SaturationSlider_sliderMoved(int position);

    void on_brightnessSlider_sliderMoved(int position);

private:
    Ui::AdvDialog *ui;
    QSlider* trying;

};


#endif // ADVDIALOG_H
