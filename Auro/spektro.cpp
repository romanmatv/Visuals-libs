#include "spektro.h"
#include "math.h"

DLLEXPORT void init(QSize size, AudioThread* tread)
{
    QSettings sets;
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(255, 255, 255));
    circlePen = QPen(Qt::black);
    circlePen.setWidth(20);
    textPen = QPen(Qt::black);
    textFont.setPixelSize(30);
    textFont.setKerning(true);

    ////

    visColor = COLOR_PALETTE [sets.value("Auro/color_pal",0).toInt()];

    bgndBrush = QBrush (QColor((TransformColor (visColor, DIM_BACKGROUND))));
    circleBrush = QBrush (QColor((TransformColor (visColor, DIM_CIRCLE))));

    // Color is changed dynamically
    // Black color is just a stub
    dotBrush = QBrush(QColor(Qt::black));
    wavePen = QPen(QColor(Qt::black), 1.0f);

    setWinSize(size);

    audio.reset(tread);
}

DLLEXPORT void destroy(){

}

DWORD TransformColor (DWORD color, short light)
{
    int a = ((color & 0xFF000000) >> 24);
    int r = ((color & 0xFF0000) >> 16) + light;
    int g = ((color & 0xFF00) >> 8) + light;
    int b = (color & 0xFF) + light;

    b = min (max (0, b), 0xFF);
    g = min (max (0, g), 0xFF);
    r = min (max (0, r), 0xFF);

    return (a << 24) | (r << 16) | (g << 8) | b;
}

QColor TransformColor (QColor color, short light){
    int r = (color.red()) + light;
    int g = (color.green()) + light;
    int b = (color.blue()) + light;

    b = min (max (0, b), 255);
    g = min (max (0, g), 255);
    r = min (max (0, r), 255);

    color.setRed(r);
    color.setGreen(g);
    color.setBlue(b);
    return color;
}

DLLEXPORT void paint(QPainter *qpainter, QPaintEvent *event){
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    qpainter->fillRect(event->rect(), bgndBrush);
    qpainter->translate(0, winSize.height()/2);

    qpainter->save();
    qpainter->setBrush(circleBrush);
    qpainter->setPen(circlePen);

    if (audio.data()){        
        if (audio.data()->isPlaying()){
            audio.data()->getFFT(fft, BASS_DATA_FFT8192);
            audio.data()->getLevels(levels);
        }else{
            for (int i=0; i<sizeFFT; i++){
                fft[i] = fft[i] / 2;
            }
            levels[0] = levels[0] / 2;
            levels[1] = levels[1] / 2;
        }
        ///////////////////////////////////////////////////////////
        qpainter->translate((winSize.width()/2), 0);

        // Draw inner circle
        int rad = innerRadius;
        float beatValue = 0.0f;
        for (int i = BEAT_ANALYZE_MIN; i <= BEAT_ANALYZE_MAX; ++i)
        {
            // Detecting beats
            // TODO improve check
            float beatValueL = levels[0];
            float beatValueR = levels[1];
            beatValue = max3(beatValue, beatValueL, beatValueR);
        }

        if (beatValue > BEAT_VALUE_THRESHOLD)
        {
            float scaleFactor = CIRCLE_K_FACTOR * beatValue + CIRCLE_B_FACTOR;
            rad = static_cast<int> (scaleFactor * rad);
        }

        qpainter->setBrush(circleBrush);
        qpainter->drawEllipse(-rad, -rad, 2 * rad, 2 * rad);

        QColor();

        for (float power = 0.5; power < 2.0; power += 0.5)
        {
            // TODO deal with all this crap
            QColor powerColor = visColor;
            if (power == 1.5)
            {
                powerColor = TransformColor (powerColor, DIM_MAX_WAVE);
            }
            else if (power == 0.5)
            {
                powerColor = TransformColor (powerColor, DIM_MIN_WAVE);
            }

            dotBrush.setColor((powerColor));
            wavePen.setColor((powerColor));

            // TODO think about drawing separate visualizations
            // for L and R channels if screen width is big enough.

            // Draw lines around the circle
            for (int i = 0; i < sizeFFT; i += lineStep)
            {
                //float v = fft[i] * power;
                float v = sqrt(fft[i])*power;

                float x = outerRadius * cos (i * CONST_VALUE);
                float y = outerRadius * sin (i * CONST_VALUE);
                float x2 = (outerRadius + v * waveAmplitude) * cos (i * CONST_VALUE);
                float y2 = (outerRadius + v * waveAmplitude) * sin (i * CONST_VALUE);

                // draw line only one time with power 1.0
                if (power == 1.0)
                {
                    qpainter->setPen(wavePen);
                    qpainter->drawLine(x, y, x2, y2);
                }

                // Draw dots at line vertex (diam = 3)
                qpainter->setBrush(dotBrush);
                qpainter->drawEllipse(x2 - 1, y2 - 1, 3.0f, 3.0f);
            }

            // Draw lines between vertexes
            for (int i = 0; i < sizeFFT; i += waveStep)
            {
                int j = i + waveStep;
                if (j >= sizeFFT)
                {
                    j = 0;
                }

                //float v1 = fft[i] * power;
                //float v2 = fft[j] * power;
                float v1 = sqrt(fft[i]) * power;
                float v2 = sqrt(fft[j]) * power;


                float x1 = (outerRadius + v1 * waveAmplitude) * cos (i * CONST_VALUE);
                float y1 = (outerRadius + v1 * waveAmplitude) * sin (i * CONST_VALUE);

                float x2 = (outerRadius + v2 * waveAmplitude) * cos (j * CONST_VALUE);
                float y2 = (outerRadius + v2 * waveAmplitude) * sin (j * CONST_VALUE);

                qpainter->setPen(wavePen);
                qpainter->drawLine(x1, y1, x2, y2);
            }
        }
        ///////////////////////////////////////////////////////////
    }else{
        qpainter->drawText(0,0, "audio treath не загружен");
    }

    qpainter->restore();
}

DLLEXPORT void animate(){
    //int posOffset[17] = {2, 3, 4, 6, 8, 10, 12, 14, 16, 20, 25, 30, 35, 60, 80, 100, 120};
}

DLLEXPORT void setWinSize(QSize s){
    winSize = s;
    WIDTH = winSize.width();
    HEIGHT = winSize.height();

    int minDimension = min(WIDTH, HEIGHT);

    outerRadius = minDimension * OUTER_RADIUS_PERCENT / 100;
    innerRadius = minDimension * INNER_RADIUS_PERCENT / 100;

    waveAmplitude = minDimension * WAVE_AMPLITUDE_PERCENT / 100;

    waveStep = minDimension / K_WAVE_STEP + B_WAVE_STEP;
    lineStep = minDimension / K_LINE_STEP + B_LINE_STEP;
}


DLLEXPORT void mouseLeftClicked(QPointF p){
    QSettings sets;
    currentColorIndex = (currentColorIndex + 1) % MAX_COLORS;
    visColor = COLOR_PALETTE [currentColorIndex];

    sets.setValue("Auro/color_pal", currentColorIndex);

    bgndBrush.setColor ((TransformColor (visColor, DIM_BACKGROUND)));
    circleBrush.setColor ((TransformColor (visColor, DIM_CIRCLE)));
}
