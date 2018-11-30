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

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define avg(a,b)            (((a) + (b)) / 2)
#define max3(a,b,c)         (max((a), (b)), (c))

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
    QString name = "Auro"; //Наименование
    QString author = "romanmatv"; //Автор
    QString description = "Auro"; //Описание
    QString version = "1.0.0"; //Версия
    QString filename = "Auro.dll"; //Имя файла
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
const int sizeFFT = 4096;
QBrush background;
QBrush circleBrush;
QFont textFont;
QPen circlePen;
QPen textPen;
QColor visColor;
float fft[sizeFFT];
float levels[2];

QPen wavePen;
QBrush dotBrush;
QBrush bgndBrush;
int max = -1000;
int min = 1000;
int WIDTH = 200;          // ширина матрицы (число диодов)
int HEIGHT = 200;         // высота матрицы (число диодов)

float CONST_VALUE = static_cast<float>(2 * M_PI / sizeFFT);

const short MAX_COLORS = 7;

const QColor COLOR_PALETTE[MAX_COLORS] =
{
    QColor(0xFFFFFFFF),
    QColor(0xFF33B5E5),
    QColor(0xFF99CB00),
    QColor(0xFFFF4444),
    QColor(0xFFFFBB33),
    QColor(0xFFA966CB),
    QColor(0xA966CBFF)
};

DWORD TransformColor (DWORD color, short light);
QColor TransformColor (QColor color, short light);


int currentColorIndex = 0;
const short DIM_MIN_WAVE = -128;
const short DIM_MAX_WAVE = -64;
const short DIM_BACKGROUND = -192;
const short DIM_CIRCLE = -96;

// Okey, Full HD
const int MAX_WIDTH = 1920;
const int MAX_HEIGTH = 1080;

const float BEAT_VALUE_THRESHOLD = 30.0f;
const short BEAT_ANALYZE_MIN = 1;
const short BEAT_ANALYZE_MAX = 2;
// factor = k * x + b
const float CIRCLE_K_FACTOR = 0.005f;
const float CIRCLE_B_FACTOR = 1.0f;

// step minDimension / k + b
const short K_LINE_STEP = -100;
const short K_WAVE_STEP = -200;
const short B_LINE_STEP = 10;
const short B_WAVE_STEP = 22;
short lineStep;
short waveStep;

// radius = minDimension * percent / 100
const USHORT OUTER_RADIUS_PERCENT = 35;
const USHORT INNER_RADIUS_PERCENT = 15;
const USHORT WAVE_AMPLITUDE_PERCENT = 30;

short outerRadius;
short innerRadius;
short waveAmplitude;

float fftMax=-10000, fftMin=10000;
/*Кастомные функции*/
static long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
static float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
template <typename T>
static T constrain(T x, T a, T b){
    if (x>=a && x<=b) return x;
    if (x<a) return a;
    if (x>b) return b;
    return a;
}
template <typename T>
static T constrain(T x, int a, int b){
    if (x>=a && x<=b) return x;
    if (x<a) return a;
    if (x>b) return b;
    return a;
}

#endif // SPEKTRO_H
