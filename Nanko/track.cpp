#include "track.h"

Track::Track(){
    m_manager = new QNetworkAccessManager(this);
    cover = QPixmap(":/imgs/no_art.jpg");
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onPage_loaded(QNetworkReply*)));
    connect(&m_pixmapLoader, SIGNAL(loaded(QPixmap)), this, SLOT(onPixmap_load(QPixmap)));
}

Track::Track(QUrl _path){
    m_manager = new QNetworkAccessManager(this);
    cover = QPixmap(":/imgs/no_art.jpg");
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onPage_loaded(QNetworkReply*)));
    connect(&m_pixmapLoader, SIGNAL(loaded(QPixmap)), this, SLOT(onPixmap_load(QPixmap)));
    path = _path;
    title = path.fileName();
}

Track::Track(QUrl _path, QString _artist, QString _title){
    m_manager = new QNetworkAccessManager(this);
    path = _path;
    artist = _artist;
    title = _title;
    search_cover(artist + " "+title);
}

Track::Track(QUrl _path, QString _radioName){
    m_manager = new QNetworkAccessManager(this);
    path = _path;
    title = path.fileName();
    radioName = _radioName;
}

Track::Track(QUrl _path, QString _artist, QString _title, QString _radioName){
    m_manager = new QNetworkAccessManager(this);
    path = _path;
    artist = _artist;
    cover = QPixmap(":/imgs/no_art.jpg");
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onPage_loaded(QNetworkReply*)));
    connect(&m_pixmapLoader, SIGNAL(loaded(QPixmap)), this, SLOT(onPixmap_load(QPixmap)));
    if (_title.isEmpty()){
        _title = _path.fileName();
    }else{
        title = _title;
        if (!path.isLocalFile() && _radioName.isEmpty()){
            radioName = _title;
        }else{
            radioName = _radioName;
        }
        search_cover(artist + " "+title);
    }

}

Track::~Track()
{
    delete m_manager;
}

QString Track::captionFormated(QString format){
    QString fooString("{title}");
    format.replace(format.indexOf(fooString), fooString.size(), title);
    fooString = "{artist}";
    format.replace(format.indexOf(fooString), fooString.size(), artist);
    fooString = "{radioName}";
    format.replace(format.indexOf(fooString), fooString.size(), radioName);
    fooString = "{radioname}";
    format.replace(format.indexOf(fooString), fooString.size(), radioName);
    fooString = "{genre}";
    format.replace(format.indexOf(fooString), fooString.size(), genre);
    fooString = "{album}";
    format.replace(format.indexOf(fooString), fooString.size(), album);
    fooString = "{date}";
    format.replace(format.indexOf(fooString), fooString.size(), date);

    return format;
}

void Track::clear(){
    title = "";
    artist = "";
    album = "";
    genre = "";
    date = "";
}

void Track::search_cover(QString search){
#ifdef QT_DEBUG
qDebug() << "Начало поиска ковера";
#endif
    parse("https://yandex.ru/images/search?text="+search+" cover art");
}

void Track::parse(QString url) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    //m_manager->post(request, "");
    m_manager->get(request);
}
void Track::onPage_loaded(QNetworkReply *reply) {
    QString buff = reply->readAll();
    QRegExp iconsRegExp("<img class=\"serp-item__thumb\" src=\"([\\w\\d\\/\\?.;=\\-&%]+);n=(\\d+)\"");
    iconsRegExp.setMinimal(true);

//#ifdef QT_DEBUG
qDebug() << "Страница загружена";
qDebug() << buff;
//#endif

    QStringList icons;

    int lastPos = 0;
    while ((lastPos = iconsRegExp.indexIn(buff, lastPos)) != -1) {
        QStringList iconData;
        lastPos += iconsRegExp.matchedLength();
//#ifdef QT_DEBUG
qDebug() << iconsRegExp.cap(1) << iconsRegExp.cap(2) << iconsRegExp.cap(3) << iconsRegExp.cap(4);
//#endif

        icons.push_back("https:"+iconsRegExp.cap(1));
    }
    emit finished(icons);
    onFinish(icons);
    reply->deleteLater();
}

void Track::onFinish(QStringList ics){
//#ifdef QT_DEBUG
qDebug() << ics.count() << " covers";
//#endif
    if (ics.count()>0){
//#ifdef QT_DEBUG
qDebug() << "Попытка загрузить по адресу "<<QString(ics[0]+"&n=500");
//#endif
        m_pixmapLoader.load(ics[0]+"&n=500");
    }else{

    }
}

void Track::onPixmap_load(QPixmap pixmap) {
//#ifdef QT_DEBUG
qDebug() << "Изображение загружено: " << pixmap.width() << "x"<<pixmap.height();
//#endif
    cover = pixmap;
}

void Track::setLocalCover(QPixmap img){
    local_cover = img;
}

void Track::setLocalCover(QImage img){
    local_cover.convertFromImage(img);
}
