#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "clientmodel.h"

#include "version.h"

// Copyright year (2009-this)
// Todo: update this when changing our copyright comments in the source

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // Set current copyright year
    ui->copyrightLabel->setText(tr("Copyright") + QString(" &copy; 2009-2014 ")+ tr("The Bitcoin developers") + QString("<br>") + 
                                tr("Copyright") + QString(" &copy; 2012 ") + tr("The Peercoin developers") + QString("<br>") +
                                tr("Copyright") + QString(" &copy; ") + tr("2014-%1 The JackpotCoin developers").arg(COPYRIGHT_YEAR));
}


void AboutDialog::setModel(ClientModel *model)
{
    if (model)
    {
        ui->versionLabel->setText(model->formatFullVersion());
    }
}


AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::on_buttonBox_accepted()
{
    close();
}
