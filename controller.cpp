#include "controller.h"
#include <QMediaPlayer>
#include <QIcon>
#include <QTime>
#include <QDebug>

Controller::Controller(QWidget *parent)
    : QHBoxLayout(parent)
{
    QHBoxLayout *tmp = new QHBoxLayout;
    tmp->setSpacing(7);
    addLayout(tmp);

    position_label = new QLabel(tr("00:00:00"), parent);
    duration_slider = new QSlider(Qt::Horizontal, parent);
    duration_label = new QLabel(tr("00:00:00"), parent);
    back_button = new QPushButton(QIcon(":/images/back.png"), tr(""), parent);
    play_button = new QPushButton(QIcon(":/images/play.png"), tr(""), parent);
    next_button = new QPushButton(QIcon(":/images/next.png"), tr(""), parent);
    volume_slider = new QSlider(Qt::Horizontal, parent);
    full_button = new QPushButton(QIcon(":/images/full.png"), tr(""), parent);


    // 在此添加 倍速
    cmbox = new QComboBox(parent);
    cmbox->addItem("1.0X");
    cmbox->addItem("2.0X");
    cmbox->show();


    position_label->setMaximumSize(64, 48);
    duration_slider->setMaximumWidth(256);
    duration_label->setMaximumSize(64, 48);
    duration_slider->setMinimumWidth(96);
    back_button->setIconSize(QSize(48, 48));
    back_button->setMaximumSize(64, 48);
    play_button->setIconSize(QSize(48, 48));
    play_button->setMaximumSize(64, 48);
    next_button->setIconSize(QSize(48, 48));
    next_button->setMaximumSize(64, 48);
    volume_slider->setMaximumWidth(120);
    volume_slider->setMinimumWidth(50);
    volume_slider->setRange(0, 100);
    volume_slider->setValue(50);
    full_button->setIconSize(QSize(48, 48));
    full_button->setMaximumSize(64, 48);

    tmp->addWidget(position_label);
    tmp->addWidget(duration_slider);
    tmp->addWidget(duration_label);
    tmp->addWidget(back_button);
    tmp->addWidget(play_button);
    tmp->addWidget(next_button);
    tmp->addWidget(volume_slider);
    tmp->addWidget(full_button);

    // 在此处添加 cmbox
    tmp->addWidget(cmbox);

    position_label->show();
    duration_slider->show();
    duration_label->show();
    back_button->show();
    play_button->show();
    next_button->show();
    volume_slider->show();
    full_button->show();


    connect(duration_slider, &QSlider::sliderMoved, this, &Controller::positionSlided);     // 用户造成 与下面valueChanged区分开
    connect(play_button, &QPushButton::clicked, this, &Controller::playClicked);
    connect(volume_slider, &QSlider::valueChanged, this, &Controller::volumeSlided);        // 只要滑条值发生改变
    connect(full_button, &QPushButton::clicked, this, &Controller::fullClicked);
    connect(cmbox, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Controller::comboBoxchose);
}

Controller::~Controller()
{
}

void Controller::stateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        play_button->setIcon(QIcon(":/images/pause.png"));
        break;
    default:
        play_button->setIcon(QIcon(":/images/play.png"));
        break;
    }
}

void Controller::playClicked()
{
    emit playChanged();
}

void Controller::volumeSlided()
{
    emit volumeChanged(volume_slider->value());
}

void Controller::fullClicked()
{
    emit fullChanged();
}

void Controller::positionSlided()                                       // 此为用户拖拽造成的滑条改变
{
    emit positionChange(qint64(duration_slider->value() * 1000));
}

void Controller::setDuration(qint64 value)                              // 视频载入时的最大时长
{
    duration_slider->setMaximum(value / 1000);
    updateDurationInfo(value);
}

void Controller::setPosition(qint64 value)                              // 视频播放中的进度，来自用户或者自然播放
{
    if(!duration_slider->isSliderDown())
        duration_slider->setValue(value / 1000);
    updateCurrentInfo(value);
}

void Controller::setVolume(int value)   // 音量
{
    volume_slider->setValue(value);
}

void Controller::updateDurationInfo(qint64 value)
{
    QTime durationTime(value/1000/60/60, value/1000/60%60, value/1000%60);
    duration_label->setText(durationTime.toString("hh:mm:ss"));
}

void Controller::updateCurrentInfo(qint64 value)
{
    QTime currentTime(value/1000/60/60, value/1000/60%60, value/1000%60);
    position_label->setText(currentTime.toString("hh:mm:ss"));
}


void Controller::comboBoxchose(int idx){
    emit comboBoxChange(idx);
}
