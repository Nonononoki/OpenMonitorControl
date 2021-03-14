/* feature_base_model.h - Records all VCP feature values for a single monitor */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURE_BASE_MODEL_H
#define FEATURE_BASE_MODEL_H

#include <QtCore/QObject>

#include "ddcutil_types.h"

#include "nongui/ddc_error.h"
#include "nongui/feature_value.h"
#include "nongui/vcprequest.h"
#ifdef FEATURE_CHANGE_OBSERVER
#include "nongui/feature_change_observer.h"
#endif
// #include "nongui/ddc_error_subject.h"

// typedef void (*NotifyFeatureChanged)(uint8_t feature_code);

class Monitor;

/** Records all VCP values for a single monitor.
 *
 *  For alternative (unused) feature views that use QListWidgets etc.
 *  this class is contained in FeatureTableModel etc.
 */
class FeatureBaseModel : public QObject // , DdcFeatureErrorSubject
{
    Q_OBJECT

public:
    FeatureBaseModel(Monitor * monitor);
    virtual ~FeatureBaseModel();

    FeatureValue* modelVcpValueFind(uint8_t feature_code);
    FeatureValue* modelVcpValueFilteredFind(uint8_t feature_code);
    FeatureValue* modelVcpValueAt(int ndx) const;
    int  modelVcpValueCount(void) const;
    void modelVcpValueSet(
             uint8_t                     feature_code,
             DDCA_Display_Ref            dref,   // unnecessary, can get from monitor
             DDCA_Feature_Metadata *     metadata,
             DDCA_Non_Table_Vcp_Value *  feature_value,
             DDCA_Status                 ddcrc);    // ano/dor full blown DDC_Error_info?
    void modelVcpValueUpdate(
             uint8_t                     feature_code,
             uint8_t                     sh,
             uint8_t                     sl);
#ifdef UNUSED
    void modelMccsVersionSet(DDCA_MCCS_Version_Spec vspec);
    DDCA_MCCS_Version_Spec
         mccsVersionSpec();
#endif
    void modelStartInitialLoad(void);
    void modelEndInitialLoad(void);
    void setStatusMsg(QString msg);
    void setCapabilities(
             DDCA_Status          ddcrc,
             char *               capabilities_string,
             DDCA_Capabilities *  parsed_capabilities);
    void dbgrpt();
#ifdef FEATURE_CHANGE_OBSERVER
 // void addFeatureChangedObserver(NotifyFeatureChanged func);
    void addFeatureChangeObserver(FeatureChangeObserver *observer);
#endif
    void setFeatureList(DDCA_Feature_List featureList);
    void setFeatureChecked(uint8_t featureCode);
    void reloadFeatures();
    void reloadSpecificFeatures(int ct, uint8_t* features);

    // *** Public Member Variables ***
    const char *        _cls;    // classNameS

    Monitor *           _monitor;

    bool                _caps_check_complete = false;
    DDCA_Status         _caps_status = -999;    // a value that's undefined
    char *              _caps_string = NULL;
    DDCA_Capabilities * _parsed_caps = NULL;

signals:
    void signalStartInitialLoad(void);
    void signalEndInitialLoad(void);
    void signalFeatureAdded(FeatureValue fv);
    // void signalFeatureUpdated(char feature_code);
    void signalFeatureUpdated3(const char * caller, uint8_t feature_code, uint8_t sh, uint8_t sl);
    void signalVcpRequest(VcpRequest * rqst);  // used to call into monitor
    void signalModelError(uint8_t featureCode, QString msg);
    void signalDdcDetailedError(DdcDetailedError * perec);
    void signalDdcFeatureError(DdcFeatureError* perec);
    void signalStatusMsg(QString msg);
    void signalCapabilitiesLoaded(void);

public slots:
    void onDdcDetailedError(DdcDetailedError * perec);
    void onDdcFeatureError(DdcFeatureError* perec);

protected:
#ifdef FEATURE_CHANGE_OBSERVER
    void notifyFeatureChangeObservers(uint8_t feature_code);
#endif

private:
    int modelVcpValueIndex(uint8_t feature_code);

    QVector<FeatureValue*> * _featureValues;
    DDCA_Feature_Metadata *  _featureMetadata[256] = {NULL};
    // DDCA_Status              _featureStatusCode[256];    // side table for now, include in FeatureValue?  UNUSED
#ifdef UNUSED
    DDCA_MCCS_Version_Spec   _vspec;
#endif

#ifdef FEATURE_CHANGE_OBSERVER
    QVector<FeatureChangeObserver*> * _featureChangeObservers;
#endif

private:
    DDCA_Feature_List   _featuresToShow;
    DDCA_Feature_List   _featuresChecked;
// ifdef FUTURE
    DDCA_Feature_List   _featuresTouchedByX14 = DDCA_EMPTY_FEATURE_LIST;
// #endif
    bool                _initialLoadActive = false;

};

#endif // FEATURE_BASE_MODEL_H
