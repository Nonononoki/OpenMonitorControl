// other_options_dialog.cpp

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <QtCore/QDebug>

#include "base/core.h"
#include "help/help_dialog.h"

#include "option_dialogs/other_options_dialog.h"
#include "ui_other_options_dialog.h"    // generated by UIC


void OtherOptionsDialog::setUiSource(NcValuesSource source)
{
   bool debugFunc = false;
   TRACEMF(debugFunc, "Executing. source=%d=%s", source, ncValuesSourceName(source) );
   switch(source) {
   case NcValuesFromCapabilities:
       ui->capabilitiesNcValuesButton->setChecked(true);
       break;
   case NcValuesFromMccs:
       ui->mccsNcValuesButton->setChecked(true);
       break;
   case NcValuesFromBoth:
      ui->bothNcValuesButton->setChecked(true);
      break;
   }
}


OtherOptionsDialog::OtherOptionsDialog(OtherOptionsState * state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherOptionsDialog),
    _cls( metaObject()->className() ),
    _state(state)
{
    // _state = state;
    ui->setupUi(this);

    setWindowTitle("Other Options");
    setUiSource(state->_ncValuesSource);
}


OtherOptionsDialog::~OtherOptionsDialog()
{
    delete ui;
}


void OtherOptionsDialog::on_buttonBox_accepted()
{
    NcValuesSource oldsrc = _state->_ncValuesSource;

    if (ui->capabilitiesNcValuesButton->isChecked() )
        _state->_ncValuesSource = NcValuesFromCapabilities;
    else if (ui->bothNcValuesButton->isChecked() )
        _state->_ncValuesSource = NcValuesFromBoth;
    else {
       assert(  ui->mccsNcValuesButton->isChecked() );
       _state->_ncValuesSource = NcValuesFromMccs;
    }

    if (_state->_ncValuesSource != oldsrc) {
        emit ncValuesSourceChanged(_state->_ncValuesSource);
    }
}


void OtherOptionsDialog::on_buttonBox_helpRequested()
{
   TRACEC("Starting");
#ifdef OLD
    // TRACE();
    QString fn(":/docs/nc_values.html");
    QFile f(fn);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&f);

    QString htmlText = in.readAll();

    // qDebug() << htmlText;

    // doesn't show dialog box
    // HelpDialog2("ddcui Help - Other Options", htmlText, this);

    HelpDialog2* hd = new HelpDialog2(this);
    hd->setText(htmlText);
    hd->setWindowTitle("ddcui Help - Other Options");
    hd->show();
    showHelp(
#endif

    viewHelp(QString("nc_values.html"), QString("ddcui Help -X Other Options"), this);
}


// Reset
void OtherOptionsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
   if(button== (QAbstractButton*) ui->buttonBox->button(QDialogButtonBox::Reset) ){
      // TRACE("Reset");
      setUiSource(OtherOptionsState::DefaultNcValuesSource);
      // on_buttonBox_accepted();  // do not actually change until OK

      // NcValuesSource oldsrc = _state->ncValuesSource;
      //  _state->ncValuesSource = OtherOptionsState::DefaultNcValuesSource;
      //   setUiSource(_state->ncValuesSource);
      //   if (_state->ncValuesSource != oldsrc) {
      //       emit ncValuesSourceChanged(_state->ncValuesSource);
      //   }
   }
}

