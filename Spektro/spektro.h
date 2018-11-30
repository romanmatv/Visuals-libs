#ifndef SPEKTRO_H
#define SPEKTRO_H

#define DLLEXPORT extern "C" __declspec(dllexport)
#include <QtCore>
#include <QtCore/qglobal.h>
#include <QPaintEvent>
#include <QWidget>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <AudioThread/AudioThread>

#define BASS_DATA_FFT256	0x80000000	// 256 sample FFT
#define BASS_DATA_FFT512	0x80000001	// 512 FFT
#define BASS_DATA_FFT1024	0x80000002	// 1024 FFT
#define BASS_DATA_FFT2048	0x80000003	// 2048 FFT
#define BASS_DATA_FFT4096	0x80000004	// 4096 FFT
#define BASS_DATA_FFT8192	0x80000005	// 8192 FFT
#define BASS_DATA_FFT16384	0x80000006	// 16384 FFT

/* Обязательные переменные */
struct Info {
    bool operator == (const Info &rhs) const{
        return (this->author==rhs.author && this->description==rhs.description && this->name == rhs.name && this->version == rhs.version);
    }
    bool isEmpty(){
        return  (name.isEmpty()&&author.isEmpty()&&description.isEmpty()&&version.isEmpty()&&filename.isEmpty());
    }
    void clear(){
        name.clear();author.clear();description.clear();version.clear();filename.clear();
    }
    QString name = "Spektro"; //Наименование
    QString author = "romanmatv"; //Автор
    QString description = "Simple Spektro"; //Описание
    QString version = "1.0.0"; //Версия
    QString filename = "Spektro.dll"; //Имя файла
    int msAni = 0; //частота вызова анимации, 0 = default = 30
} info;
QSize winSize;
QScopedPointer<AudioThread> audio;
/* Обязательные функции */
DLLEXPORT void init(QSize size, AudioThread* tread);
DLLEXPORT void destroy();
DLLEXPORT void paint(QPainter *painter, QPaintEvent *event);
DLLEXPORT void animate();
DLLEXPORT void setWinSize(QSize s){winSize = s;}
DLLEXPORT Info getInfo(){return info;}
/*Кастомные перменные*/
QBrush background;
QBrush circleBrush;
QFont textFont;
QPen circlePen;
QPen textPen;
const int sizeFFT = 4096;
int max = -1000;
int min = 1000;
float fftMax=-10000, fftMin=10000;
float last_fft[sizeFFT];
float fft[sizeFFT];
/*Кастомные функции*/
static long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
static float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // SPEKTRO_H
