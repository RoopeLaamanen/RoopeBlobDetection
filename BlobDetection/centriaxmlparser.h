#ifndef CENTRIAXMLPARSER_H
#define CENTRIAXMLPARSER_H

#include <QObject>
#include <QMap>

class CentriaXMLParser
{
public:
    CentriaXMLParser();

    static QMap<QString, QString> ParseXMLContent(QString mainTag, QString xmlContent);
};

#endif // CENTRIAXMLPARSER_H
