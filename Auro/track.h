#ifndef TRACK_H
#define TRACK_H
#include <QtCore>
#include <QObject>
#include "pixmaploader.h"
#include <QtNetwork/QtNetwork>

class Track : public QObject
{
    Q_OBJECT
public:
    explicit Track();
    explicit Track(QUrl _path);
    explicit Track(QUrl _path, QString _radioName);
    explicit Track(QUrl _path, QString _artist, QString _title);
    explicit Track(QUrl _path, QString _artist, QString _title, QString _radioName);
    ~Track();
    QString artist = "";
    QString title = "";
    QString album = "";
    QUrl path = QUrl();
    QString durationStr = "";
    QString genre = "";
    QString date = "";
    QString Format = "";
    QString radioName = "";

    bool active = false;

    QPixmap cover;
    QPixmap local_cover;
    PixmapLoader m_pixmapLoader;

    void clear();
    void search_cover(QString);
    QString captionFormated(QString format);


public slots:
    void setLocalCover(QPixmap);
    void setLocalCover(QImage);
private slots:
    void onPage_loaded(QNetworkReply*);
    void onFinish(QStringList);
    void onPixmap_load(QPixmap);
    void parse(QString);
signals:
     void finished(QStringList);
private:
     QNetworkAccessManager *m_manager;
};
Q_DECLARE_METATYPE(Track *)
#endif // TRACK_H
