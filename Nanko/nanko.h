#ifndef NANKO_H
#define NANKO_H

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

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define avg(a,b)            (((a) + (b)) / 2)


// анимация
#define SMOOTH 0.4        // плавность движения столбиков (0 - 1)
#define DELAY 4           // задержка между обновлениями матрицы (периодичность основного цикла), миллиисекунды

// точки максимума
#define MAX_DOTS 1        // включить/выключить отрисовку точек максимума (1 вкл, 0 выкл)
#define FALL_DELAY 5     // скорость падения точек максимума (задержка, миллисекунды)
#define FALL_PAUSE 70    // пауза перед падением точек максимума, миллисекунды

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
    QString name = "z double Nanko"; //Наименование
    QString author = "romanmatv"; //Автор
    QString description = "Nanko"; //Описание
    QString version = "1.0.0"; //Версия
    QString filename = "Nanko.dll"; //Имя файла
    int msAni = 0; //частота вызова анимации, 0 = default = 30
} info;
QSize winSize;
QScopedPointer<AudioThread> audio;
/* Обязательные функции */
DLLEXPORT void init(QSize size, AudioThread* tread);
DLLEXPORT void destroy();
DLLEXPORT void paint(QPainter *painter, QPaintEvent *event);
DLLEXPORT void animate();
DLLEXPORT void setWinSize(QSize s);
DLLEXPORT Info getInfo(){return info;}

DLLEXPORT void mouseLeftDoubleClicked(QPointF){}
DLLEXPORT void mouseRightDoubleClicked(QPointF){}
DLLEXPORT void mouseLeftClicked(QPointF);
DLLEXPORT void mouseRightClicked(QPointF){}
/*Кастомные перменные*/
QBrush background;
QBrush circleBrush;
QPen circlePen;
QPen textPen;

double ten;
double eght;

const int sizeFFT = 128;
const int countBars = 34;

int posOffset[countBars] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 32, 36, 40, 44, 48, 52, 57, 62, 67, 72, 78, 84, 90, 96, 102, 108, 120, 127
};

float fft[sizeFFT];

unsigned long gainTimer, fallTimer;
double maxLevel[countBars];
double posLevel_old[countBars];
unsigned long timeLevel[countBars];
boolean fallFlag;


double fftMax=-10000, fftMin=10000;
/*Кастомные функции*/
template <typename T>
static T map(T x, T in_min, T in_max, T out_min, T out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//Найденная на просторах функция (работает лучше)
template <typename T>
T convert(T value, T From1, T From2, T To1, T To2)
{
      return (value-From1)/(From2-From1)*(To2-To1)+To1;
}
template <typename T>
static T constrain(T x, T a, T b){
    if (x<a) return a;
    if (x>b) return b;
    return x;
}

#endif // SPEKTRO_H
