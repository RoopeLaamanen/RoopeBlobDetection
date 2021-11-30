#include "centriaxmlparser.h"

CentriaXMLParser::CentriaXMLParser()
{

}

QMap<QString, QString> CentriaXMLParser::ParseXMLContent(QString mainTag, QString xmlContent)
{
    QMap<QString, QString> parsedContent;

    QString startMainTag = QString("<%1>").arg(mainTag);
    QString endMainTag = QString("</%1>").arg(mainTag);

    int startMainTagPosition = xmlContent.indexOf(startMainTag);
    if(startMainTagPosition>=0)
    {
        int endMainTagPosition = xmlContent.indexOf(endMainTag);
        if(endMainTagPosition > startMainTagPosition)
        {
            int startContent = startMainTagPosition + startMainTag.size();
            int lengthContent = endMainTagPosition - startContent;
            if(lengthContent > 0)
            {
                QString content = xmlContent.mid(startContent,lengthContent);
                int openMark = content.indexOf("<");
                while(openMark >= 0)
                {
                    int closeMark = content.indexOf(">",openMark);
                    if(closeMark > openMark)
                    {
                        QString tagName = content.mid(openMark + 1, closeMark - openMark - 1);
                        if(!tagName.isEmpty())
                        {
                            QString endTag = QString("</%1").arg(tagName);
                            int endPosition = content.indexOf(endTag,closeMark);
                            if(endPosition > closeMark)
                            {
                                int lenght = endPosition - closeMark - 1;
                                QString tagValue = content.mid(closeMark + 1, lenght);
                                if(!tagValue.isEmpty())
                                {
                                    parsedContent.insert(tagName, tagValue);
                                    content = content.mid(endPosition + endTag.size(), content.size()- endPosition - endTag.size());
                                }
                            }
                            else
                            {
                                content = content.mid(closeMark, content.size()-closeMark);
                            }
                        }
                        else
                        {
                            content = content.mid(closeMark, content.size()-closeMark);
                        }
                    }
                    else
                    {
                        break;
                    }
                    openMark = content.indexOf("<");
                }
            }
        }
    }

    return parsedContent;
}
