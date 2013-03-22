#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>

namespace inviwo {

FloatPropertyWidgetQt::FloatPropertyWidgetQt(FloatProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatPropertyWidgetQt::generateWidget() {
    
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    slider_ = new FloatSliderQt(property_->getMinValue(), property_->getMaxValue());
    spinBox_ = new QDoubleSpinBox();
    spinBox_->setRange(static_cast<double>(property_->getMinValue()),static_cast<double>(property_->getMaxValue()));
    spinBox_->setSingleStep(static_cast<double>(property_->getIncrement()));
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSlider()));
    //connect (spinBox_,SIGNAL(valueChanged(double)), this, SLOT(setPropertyValueFromSpinbBox()));
    hLayout->addWidget(slider_);
    hLayout->addWidget (spinBox_);
    setLayout(hLayout);
    
    slider_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(slider_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));

    settingsWidget_ = new PropertySettingsWidgetQt(property_);

}

void FloatPropertyWidgetQt::setPropertyValueFromSlider() {

    property_->set(slider_->getValue());
    spinBox_->setValue(static_cast<double>(slider_->getValue()));
}
void FloatPropertyWidgetQt::setPropertyValueFromSpinbBox() {
    property_->set(static_cast<float>(spinBox_->value()));
    slider_->setValue(static_cast<float>(spinBox_->value()));
}

void FloatPropertyWidgetQt::updateFromProperty() {
    slider_->setRange(property_->getMinValue(), property_->getMaxValue());
    spinBox_->setRange(static_cast<double>(property_->getMinValue()),static_cast<double>(property_->getMaxValue()));
    slider_->setValue(property_->get());
    spinBox_->setValue(static_cast<double>(property_->get()));
}

void FloatPropertyWidgetQt::showContextMenu(const QPoint& pos) {

    QPoint globalPos = slider_->mapToGlobal(pos);

    QMenu settingsMenu;

    settingsMenu.addAction("Property settings");
    settingsMenu.addAction("Set as Min");
    settingsMenu.addAction("Set as Max");

    QAction* selecteditem = settingsMenu.exec(globalPos);
    if (selecteditem == settingsMenu.actions().at(0)) {
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu.actions().at(1)) {
        //Set current value of the slider to min value of the property
        property_->setMinValue(slider_->getValue());
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu.actions().at(2)){
    //Set current value of the slider to max value of the property
        property_->setMaxValue(slider_->getValue());
        updateFromProperty();
    }
}

} // namespace
