#include "PresetWizardIntroPage.h"

PresetWizardIntroPage::PresetWizardIntroPage(QWidget *parent)
{
    Q_UNUSED(parent);
    this->setTitle(tr("Preset editor"));
    this->setSubTitle(tr("Specify if you wanna add or edit a preset"));
    _layout = new QGridLayout(this);
    _radioAdd = new QRadioButton(tr("Add new preset")),
    _radioEdit = new QRadioButton(tr("Edit existing preset"));
    _pastorLUL = new QLabel;
   // _pastorLUL->setPixmap(QPixmap(":/icon/resources/pastorLUL.jpg"));
   // _pastorLUL->setMaximumSize(300,500);
   // _pastorLUL->setScaledContents(true);

   // _layout->addWidget(_pastorLUL,0,0,8,3);


    _layout->addWidget(_radioAdd,0,0,1,6);
    _layout->addWidget(_radioEdit,1,0,1,6);

    _spoilerSFXcomboBox = new QComboBox();
    _spoilerSFX = new Spoiler(tr("Which SFX?"));
    _spoilerSFXLayout = new QGridLayout();
    _spoilerSFXLayout->addWidget(_spoilerSFXcomboBox);
    _spoilerSFXcomboBox->setMinimumHeight(20);
    _spoilerSFX->setContentLayout(_spoilerSFXLayout);
    _layout->addWidget(_spoilerSFX,2,0,1,6);
    _spoilerSFX->setEnabled(false);
    _spoilerSFXcomboBox->clear();
    _spoilerSFXcomboBox->addItem(tr("<No Item Selected>"));
    _spoilerSFXcomboBox->addItems( LIDL::PresetController::GetInstance()->GetExistingSFXList());
    _spoilerSFX->hide();

    _presetSFXSpoiler =  new Spoiler(tr("Which preset?"));
    _presetSFXcomboBox = new QComboBox();
    _presetSFXLayout  = new QGridLayout();
    _presetSFXcomboBox->setMinimumHeight(20);
    _presetSFXLayout->addWidget(_presetSFXcomboBox);
    _presetSFXSpoiler->setContentLayout(_presetSFXLayout);
    _layout->addWidget(_presetSFXSpoiler,3,0,1,6);
    _presetSFXSpoiler->hide();
    connect(_radioAdd,&QRadioButton::clicked, this , [=]{
            this->radioWasClicked();
        });

    connect(_radioEdit,&QRadioButton::clicked, this, [=]{
            this->radioWasClicked();
        });

    connect(_spoilerSFXcomboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this               , &PresetWizardIntroPage::comboBoxIndexChanged);





    _settingsSpoiler = new Spoiler("Settings");
    _settingsLayout  = new QGridLayout();


    _layout->addWidget(_settingsSpoiler,4,0,1,6);
    _settingsSpoiler->hide();
    this->addWidgets();

    _finishButton = new QPushButton(tr("Finish !"));
    _finishButton->setEnabled(false);
    _cancelButton = new QPushButton(tr("Cancel"));
    connect(_cancelButton,&QPushButton::clicked, parent,&QWizard::close);

    _layout->addItem(new QSpacerItem(10,600),5,0,5,5);
    _layout->addWidget(_finishButton,10,4,1,1);
    _layout->addWidget(_cancelButton,10,5,1,1);



}

void PresetWizardIntroPage::radioWasClicked()
{

    _settingsSpoiler->close();
    _settingsSpoiler->hide();
    _spoilerSFX->show();
    _spoilerSFX->setEnabled(true);
    _spoilerSFX->Open();
    int index = _spoilerSFXcomboBox->currentIndex();
    if (_radioAdd->isChecked())
    {
        _presetSFXSpoiler->close();
        _presetSFXSpoiler->hide();
        if (index == 0)
        {
            _settingsSpoiler->close();
            _settingsSpoiler->hide();
            return;
        }
    }
    this->comboBoxIndexChanged(_spoilerSFXcomboBox->currentIndex());
}

void PresetWizardIntroPage::comboBoxIndexChanged(int index)
{
     //_settingsSpoiler->setContentLayout(_settingsLayout);
    // hiding every widget
    _distortionWidget->hide();
    _chorusWidget->hide();
    _echoWidget->hide();
    _compressorWidget->hide();
    _flangerWidget->hide();
    _gargleWidget->hide();
    _settingsSpoiler->close();
    _settingsSpoiler->hide();
    _presetSFXSpoiler->close();
    _presetSFXSpoiler->hide();
    if (index == 0)
        return;
    // if radio Edit isn't checked we display the correct widget
    if (_radioAdd->isChecked())
    {
        _settingsSpoiler->show();
        _settingsSpoiler->Open();
        if (index == 1)
            _distortionWidget->show();
        else if (index == 2)
            _chorusWidget->show(); //2
        else if (index == 3)
            _echoWidget->show(); //3
        else if (index == 4)
            _compressorWidget->show();
        else if (index == 5)
            _flangerWidget->show();
        else if (index == 6)
            _gargleWidget->show();
    }
    else
    {

        this->_presetSFXSpoiler->show();
        this->_presetSFXSpoiler->Open();
        LIDL::SFX_TYPE type = LIDL::SFX_TYPE::NONE; // no items selected

        if (index == 1)
            type = LIDL::SFX_TYPE::DISTORTION; // 1
        else if (index == 2)
            type = LIDL::SFX_TYPE::CHORUS; //2
        else if (index == 3)
            type = LIDL::SFX_TYPE::ECHO; //3
        else if (index == 4)
            type = LIDL::SFX_TYPE::COMPRESSOR;
        else if (index == 5)
            type = LIDL::SFX_TYPE::FLANGER;
        else if (index == 6)
            type = LIDL::SFX_TYPE::GARGLE;
        this->_presetSFXcomboBox->clear();
        this->_presetSFXcomboBox->addItems(
                LIDL::PresetController::GetInstance()->GetExistingPresetsList(type));
    }
}

void PresetWizardIntroPage::addWidgets()
{
    _presetName = new QLineEdit();
    _settingsLayout->addWidget(_presetName,0,0,1,6);
    /*****************************************************/
    /*              REVAMPED  DISTORTION                 */
    /*****************************************************/

    _distortionWidget = new SfxSettingsWidget(tr("Distortion"),LIDL::SFX_TYPE::DISTORTION,false);
    _distortionWidget->addSlider(tr("Gain"),0,60, " dB",static_cast<int>(LIDL::SFX_DIST_PARAM::fGain)," -");
    _distortionWidget->addSlider(tr("Edge"),0 ,100 ," %",static_cast<int>(LIDL::SFX_DIST_PARAM::fEdge));
    _distortionWidget->addSlider(tr("Center Frequency"),100,8000," Hz",static_cast<int>(LIDL::SFX_DIST_PARAM::fPostEQCenterFrequency));
    _distortionWidget->addSlider(tr("Bandwidth"),100,8000, " Hz",static_cast<int>(LIDL::SFX_DIST_PARAM::fPostEQBandwidth));
    _distortionWidget->addSlider(tr("Lowpass Cutoff"),100,8000," Hz",static_cast<int>(LIDL::SFX_DIST_PARAM::fPreLowpassCutoff));
//    _distortionWidget->addSpacer();
     _settingsLayout->addWidget(_distortionWidget,1,0,1,6);

    /*****************************************************/
    /*              REVAMPED  CHORUS                     */
    /*****************************************************/
    _chorusWidget = new SfxSettingsWidget(tr("Chorus"),LIDL::SFX_TYPE::CHORUS,false);
    // check EnumsAndStructs.h for special value
    _chorusWidget->addSlider(tr("Delay"),0,20," ms", static_cast<int>(LIDL::SFX_CHORUS_PARAM::fDelay));
    _chorusWidget->addSlider(tr("Depth"),0,100," %",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fDepth));
    _chorusWidget->addSlider(tr("Feedback"),-99,99," %",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fFeedback));
    _chorusWidget->addSlider(tr("Frequency"),0,10," Hz",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fFrequency));
    _chorusWidget->addSlider(tr("Wet Dry Mix"),0,100," %",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fWetDryMix));
    _chorusWidget->addComboBox(tr("Phase Differential"),
                               (QStringList()  << "-180° (-π rad)"   //    BASS_FX_PHASE_NEG_180
                                               << "-90° (-π/2 rad)"  //    BASS_FX_PHASE_NEG_90
                                               <<"0° (0 rad)"        //    BASS_FX_PHASE_ZERO
                                               <<"90° (π rad)"       //BASS_FX_PHASE_90
                                               <<"180° (π/2 rad)"),
                               static_cast<int>(LIDL::SFX_CHORUS_PARAM::lPhase)); //BASS_FX_PHASE_180

    _chorusWidget->addComboBox("Wave Form", (QStringList() <<tr("Triangular Wave")
                                                  <<  tr("Sinusoidal Wave")),static_cast<int>(LIDL::SFX_CHORUS_PARAM::lWaveform));

     _settingsLayout->addWidget(_chorusWidget,1,0,1,6); // 1
    /*****************************************************/
    /*                     ECHO                          */
    /*****************************************************/
    _echoWidget = new SfxSettingsWidget(tr("Echo"),LIDL::SFX_TYPE::ECHO,false);
    _echoWidget->addSlider(tr("Feedback"),0,100," %", static_cast<int>(LIDL::SFX_ECHO_PARAM::fFeedback) );
    _echoWidget->addSlider(tr("Left Delay"),1,2000," ms", static_cast<int>(LIDL::SFX_ECHO_PARAM::fLeftDelay) );
    _echoWidget->addSlider(tr("Right Delay"),1,2000," ms", static_cast<int>(LIDL::SFX_ECHO_PARAM::fRightDelay) );
    _echoWidget->addSlider(tr("Wet Dry Mix"),0,100," %", static_cast<int>(LIDL::SFX_ECHO_PARAM::fWetDryMix) );
    _echoWidget->addComboBox(tr("Swap left and right\n"
                                "delay after echo"),
                             (QStringList() << tr("No") << tr("Yes")),
                             static_cast<int>(LIDL::SFX_ECHO_PARAM::lPanDelay));
     _settingsLayout->addWidget(_echoWidget,1,0,1,6); //2

    /*****************************************************/
    /*                  COMPRESSOR                   */
    /*****************************************************/
    _compressorWidget = new SfxSettingsWidget(tr("Dynamic\nRange Compression"),LIDL::SFX_TYPE::COMPRESSOR,false);
    _compressorWidget->addSlider(tr("Attack"),0,500," ms", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fAttack) );
    _compressorWidget->addSlider(tr("Gain"),-60,60," dB", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fGain) );
    _compressorWidget->addSlider(tr("Pre Delay"),0,4," ms", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fPredelay) );

    _compressorWidget->addSlider(tr("Ratio"),1,100,":1", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fRatio) );
    _compressorWidget->addSlider(tr("Release"),50,3000," ms", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fRelease) );
    _compressorWidget->addSlider(tr("Threshold"),-60,0," dB", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fThreshold) );

     _settingsLayout->addWidget(_compressorWidget,1,0,1,6);
    /*****************************************************/
    /*                     FLANGER                       */
    /*****************************************************/
    _flangerWidget = new SfxSettingsWidget(tr("Flanger"),LIDL::SFX_TYPE::FLANGER,false);
    _flangerWidget->addSlider(tr("Delay"),0,4," ms", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fDelay) );
    _flangerWidget->addSlider(tr("Depth"),0,100," ‱", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fDepth) );
    _flangerWidget->addSlider(tr("Feedback"),-99,99," %", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fFeedback) );
    _flangerWidget->addSlider(tr("Frequency"),0,10," Hz", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fFrequency) );
    _flangerWidget->addSlider(tr("Wet Dry Mix"),0,100," %", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fWetDryMix) );
    _flangerWidget->addComboBox(tr("Phase Differential"),
                               (QStringList()  << "-180° (-π rad)"   //    BASS_FX_PHASE_NEG_180
                                               << "-90° (-π/2 rad)"  //    BASS_FX_PHASE_NEG_90
                                               <<"0° (0 rad)"        //    BASS_FX_PHASE_ZERO
                                               <<"90° (π rad)"       //BASS_FX_PHASE_90
                                               <<"180° (π/2 rad)"),
                               static_cast<int>(LIDL::SFX_FLANGER_PARAM::lPhase)); //BASS_FX_PHASE_180

    _flangerWidget->addComboBox(tr("Wave Form"), (QStringList() <<tr("Triangular Wave")
                                              <<  tr("Sinusoidal Wave")),static_cast<int>(LIDL::SFX_FLANGER_PARAM::lWaveform));
     _settingsLayout->addWidget(_flangerWidget,1,0,1,6);
    /*****************************************************/
    /*    GARGLE (the cum like a bitch boi gachiBASS     */
    /*****************************************************/
    _gargleWidget = new SfxSettingsWidget(tr("Gargle"),LIDL::SFX_TYPE::GARGLE,false);
    _gargleWidget->addSlider(tr("Modulation rate"),1,1000,"Hz",static_cast<int>(LIDL::SFX_GARGLE_PARAM::dwRateHz));
    _gargleWidget->addComboBox(tr("Wave Form"), (QStringList() <<tr("Triangular Wave")
                                              <<  tr("Sinusoidal Wave")),static_cast<int>(LIDL::SFX_GARGLE_PARAM::dwWaveShape));
    _gargleWidget->addSpacer();
     _settingsLayout->addWidget(_gargleWidget,1,0,1,6);


     _settingsSpoiler->setContentLayout(_settingsLayout);
}
