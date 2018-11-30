#include "spektro.h"
#include "math.h"

DLLEXPORT void init(QSize size, AudioThread* tread)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(255, 255, 255));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(2);
    textPen = QPen(Qt::black);
    textFont.setPixelSize(30);
    textFont.setKerning(true);

    winSize = size;
    audio.reset(tread);
}

DLLEXPORT void destroy(){

}

DLLEXPORT void paint(QPainter *qpainter, QPaintEvent *event){
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    qpainter->fillRect(event->rect(), background);
    qpainter->translate(0, winSize.height()/2);

    qpainter->save();
    qpainter->setBrush(circleBrush);

    if (audio.data()){
        if (audio.data()->isPlaying()){
            audio.data()->getFFT(fft, BASS_DATA_FFT8192);
        }else{
            for (int i=0; i<sizeFFT; i++){
                fft[i] = fft[i] / 2;
            }
        }
        int per = sizeFFT / winSize.width();
        int lastAver = -(winSize.height()/2);
        float aver;
        int lastJ = 0;

        if (per>0){
            qpainter->setPen(circlePen);

            for (int i=0; i<winSize.width(); i++){
                aver = 0.0;
                for (int j=lastJ; j<lastJ+per; j++){
                    aver += sqrt(fft[i])*3*winSize.height()-4;
                    //qDebug() << "fft: "<<fft[j];
                }
                aver = aver / per;

                //qDebug() << "aver: " << aver;

                if (fftMin>aver) fftMin = aver;
                if (fftMax<aver) fftMax = aver;

                aver = -map(aver, fftMin, fftMax, -(winSize.height()/2), winSize.height()/2);

                lastJ = lastJ+per;
                qpainter->drawLine(i-1,lastAver, i, aver);
                lastAver = aver;
            }
        }
    }else{
        qpainter->drawText(0,0, "audio treath не загружен");
    }

    qpainter->restore();
}

DLLEXPORT void animate(){
    /*Nothing*/
}
