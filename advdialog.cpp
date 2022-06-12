#include "advdialog.h"
#include "ui_advdialog.h"

QVideoWidget *v;



AdvDialog::AdvDialog(QWidget *parent, QVideoWidget *vw) :
    QDialog(parent),
    ui(new Ui::AdvDialog)
{
    ui->setupUi(this);
    v = vw;
    this->setWindowTitle("Advanced");


}

AdvDialog::~AdvDialog()
{
    delete ui;
}


void AdvDialog::on_HueSlider_sliderMoved(int position)
{
    v->setHue(position);
}

void AdvDialog::on_ContrastSlider_sliderMoved(int position)
{
    v->setContrast(position);
}

void AdvDialog::on_SaturationSlider_sliderMoved(int position)
{
    v->setSaturation(position);
}

void AdvDialog::on_brightnessSlider_sliderMoved(int position)
{
    v->setBrightness(position);
}
