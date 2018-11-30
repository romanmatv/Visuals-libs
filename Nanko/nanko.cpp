#include "nanko.h"
#include "math.h"

DLLEXPORT void init(QSize size, AudioThread* tread)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, QColor(0xa6, 0xce, 0x39));
    gradient.setColorAt(1.0, Qt::red);

    background = QBrush(QColor(0x19232D));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(2);
    textPen = QPen(Qt::red);
    textPen.setWidth(2);

    setWinSize(size);

    audio.reset(tread);
}

DLLEXPORT void destroy(){

}

DLLEXPORT void setWinSize(QSize s){
    winSize = s;

    ten = 10.0 / 330.0 * (double)winSize.width();
    eght = 8.0 / 330.0 * (double)winSize.width();
}


DLLEXPORT void mouseLeftClicked(QPointF p){

}

double fun(double a){
    return sqrt(a)*3*winSize.height()-4;
}

DLLEXPORT void paint(QPainter *qpainter, QPaintEvent *event){
    QLinearGradient gradient(QPointF(40, 100), QPointF(-100, -40));
    gradient.setColorAt(0.0, QColor(0xa6, 0xce, 0x39));
    gradient.setColorAt(1.0, Qt::red);
    circleBrush = QBrush(gradient);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    qpainter->fillRect(event->rect(), background);
    qpainter->translate(0, winSize.height());

    qpainter->save();
    qpainter->setBrush(circleBrush);

    if (audio.data()){
        if (audio.data()->isPlaying()){
            audio.data()->getFFT(fft, BASS_DATA_FFT256);
        }else{
            for (int i=0; i<sizeFFT; i++){
                fft[i] = fft[i] * 0.3;
            }
        }

        //Ширина 330, при pos*10 и pos*10+8

        for (int pos=0; pos<countBars-1; pos++){
            qpainter->setPen(circlePen);
            double posLevel = fun(fft[posOffset[pos]]);
              int linesBetween;
              if (pos > 0 && pos < countBars-1) {
                linesBetween = posOffset[pos] - posOffset[pos - 1];
                for (byte i = 0; i < linesBetween; i++) {  // от предыдущей полосы до текущей
                  posLevel += (float) ((float)i / linesBetween) * fun(fft[posOffset[pos] - linesBetween + i]);
                }
                linesBetween = posOffset[pos + 1] - posOffset[pos];
                for (byte i = 0; i < linesBetween; i++) {  // от предыдущей полосы до текущей
                  posLevel += (float) ((float)i / linesBetween) * fun(fft[posOffset[pos] + linesBetween - i]);
                }
              }
              // найти максимум из пачки тонов
              if (fftMin>posLevel) fftMin = posLevel;
              if (fftMax<posLevel) fftMax = posLevel;

              // фильтрация длины столбиков, для их плавного движения
              posLevel = posLevel_old[pos] * SMOOTH + posLevel * (1 - SMOOTH);
              posLevel_old[pos] = posLevel;

              // преобразовать значение величины спектра в диапазон 0..7 с учётом настроек
              posLevel = -convert(posLevel, fftMin, fftMax, 0.0, (double)winSize.height());

              // если столбик больше нуля, то начать линию с координаты
              qpainter->drawRect(QRectF(pos*ten, 0.0, eght, posLevel));

              if (posLevel < maxLevel[pos]) {    // если для этой полосы есть максимум, который больше предыдущего
                maxLevel[pos] = posLevel;                        // запомнить его
                timeLevel[pos] = clock();                       // запомнить время
              }

              if (maxLevel[pos] <= 0){
                  qpainter->setPen(textPen);
                  //qpainter->drawLine(pos*10,maxLevel[pos], pos*10+8, maxLevel[pos]);
                  qpainter->drawLine(pos*ten,maxLevel[pos], pos*ten+eght, maxLevel[pos]);
              }

              if (fallFlag) {                                           // если падаем на шаг
                if ((long)clock() - timeLevel[pos] > FALL_PAUSE) {     // если максимум держался на своей высоте дольше FALL_PAUSE
                  if (maxLevel[pos] <= 0) maxLevel[pos]++;             // уменьшить высоту точки на 1
                }
              }
        }

        fallFlag = 0;                                 // сбросить флаг падения
        if (clock() - fallTimer > FALL_DELAY) {      // если настало время следующего падения
          fallFlag = 1;                               // поднять флаг
          fallTimer = clock();
        }
    }else{
        qpainter->drawText(0,0, "audio treath не загружен");
    }

    qpainter->restore();
}

DLLEXPORT void animate(){
    /*Nothing*/
}
