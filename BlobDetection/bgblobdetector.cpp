#include "bgblobdetector.h"

BGBlobDetector::BGBlobDetector(QObject *parent) : QObject(parent)
{
    LoadConfigurationFile();

    setlocale(LC_ALL,"C");
    _ocr = new tesseract::TessBaseAPI();
    _ocr->Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY);
    _ocr->SetPageSegMode(tesseract::PSM_AUTO);
}

BGBlobDetector::~BGBlobDetector()
{
    Stop();
    if(_videoCapture.isOpened())
    {
        _videoCapture.release();
    }

    if(_ocr != nullptr)
    {
        _ocr->End();
        delete _ocr;
    }
}

void BGBlobDetector::Start()
{
    if(_timerId == -1)
    {
        cv::namedWindow("image");
        cv::setMouseCallback("image", &MouseCallBack,this);

        _timerId = startTimer(100);
    }
}

void BGBlobDetector::Stop()
{
    if(_timerId >= 0)
    {
        killTimer(_timerId);
        _timerId = -1;
    }
}

void BGBlobDetector::timerEvent(QTimerEvent *event)
{
    LoadConfigurationFile();

    if(!_videoCapture.isOpened())
    {
        _videoCapture.open("/dev/video0");
        //_videoCapture.open(0);
        //_videoCapture.open("/home/centria/projects/SulautetutJarjestelmat/BlobDetection/TestVideo.mp4");
    }
    else
    {
        cv::Mat image;
        _videoCapture.read(image);
        //image = cv::imread("/home/centria/projects/SulautetutJarjestelmat/RESTAPI/Image1.jpg");


        if(image.cols > 0)
        {
            //cv::imwrite("/home/centria/projects/SulautetutJarjestelmat/RESTAPI/Image1.jpg", image);
            cv::Mat grayImage;
            cv::cvtColor(image,grayImage, cv::COLOR_BGR2GRAY);


            cv::SimpleBlobDetector::Params params;
            params.minThreshold = MinThreshold;
            params.maxThreshold = MaxThreshold;
            params.filterByArea = FilterbyArea;
            params.minArea = MinArea;
            params.maxArea = MaxArea;
            params.filterByCircularity = FilterByCircularity;
            params.minCircularity = MinCircularity;
            params.maxCircularity = MaxCircularity;
            params.filterByInertia = FilterByInertia;
            params.minInertiaRatio = MinInertia;
            params.maxInertiaRatio = MaxInertia;


            _detector = cv::SimpleBlobDetector::create(params);



            std::vector<cv::KeyPoint> keypoints;
            _detector->detect(grayImage, keypoints);

            cv::Mat im_with_keypoints;
            if(keypoints.size() > 0)
            {
                cv::Mat keypointsImage(grayImage.rows,grayImage.cols, CV_8UC3, cv::Scalar(255,0,255));

                foreach(cv::KeyPoint keypoint, keypoints)
                {
                    int radian = keypoint.size / 2;
                    QString sizetext = QString("Size:%1").arg(3.14 * radian *radian);
                    cv::circle(keypointsImage,keypoint.pt,radian,cv::Scalar(255,0,0));
                    cv::circle(image,keypoint.pt,radian,cv::Scalar(255,255,255),2);
                    cv::putText(image,sizetext.toUtf8().constData(),cv::Point(keypoint.pt.x, keypoint.pt.y -10),1,1,cv::Scalar(255,0,0));
                    //cv::line(keypointsImage,cv::Point(keypoint.pt.x - radian, keypoint.pt.y),cv::Point(keypoint.pt.x + radian, keypoint.pt.y),cv::Scalar(255,0,0));

                }

                ContentText = "";
                Confidence = -1;
                Angle = -1;
                PositionX = -1;
                PositionY = -1;
                if(keypoints.size() == 4)
                {
//                    float x1 = keypoints[0].pt.x;
//                    float x2 = keypoints[1].pt.x;
//                    float x3 = keypoints[2].pt.x;
//                    float y1 = keypoints[0].pt.y;
//                    float y2 = keypoints[1].pt.y;
//                    float y3 = keypoints[2].pt.y;

//                    float distance1 = qSqrt(((x1-x2) *(x1-x2)) + ((y1-y2)* (y1-y2)));
//                    float distance2 = qSqrt(((x1-x3) *(x1-x3)) + ((y1-y3)* (y1-y3)));
//                    float distance3 = qSqrt(((x3-x2) *(x3-x2)) + ((y3-y2)* (y3-y2)));
//                    cv::Point distance1Point = cv::Point((x1+x2)/2, (y1+y2)/2);
//                    cv::Point distance2Point = cv::Point((x1+x3)/2, (y1+y3)/2);
//                    cv::Point distance3Point = cv::Point((x3+x2)/2, (y3+y2)/2);
//                    cv::putText(keypointsImage,std::to_string(distance1),distance1Point,1,1,cv::Scalar(255,0,0));
//                    cv::putText(keypointsImage,std::to_string(distance2),distance2Point,1,1,cv::Scalar(255,0,0));
//                    cv::putText(keypointsImage,std::to_string(distance3),distance3Point,1,1,cv::Scalar(255,0,0));


                    //Perspective correction
//                    cv::Mat contentAreaImage = cv::Mat(100,200, CV_8UC1, cv::Scalar(255,255,255));
//                    std::vector<cv::Point2f> inputPoints, outputPoints;

                    //find the biggest and smallest points
                    int biggestPointIndex = 0;
                    int smallestPointIndex = 0;

                    for(int i = 1; i < 4; i++)
                    {
                        if(keypoints[i].size > keypoints[biggestPointIndex].size)
                        {
                            biggestPointIndex = i;
                        }

                        if(keypoints[i].size < keypoints[smallestPointIndex].size)
                        {
                            smallestPointIndex = i;
                        }
                    }

                    int topLeftPointIndex = 0;
                    int bottomRightIndex = 0;
                    int leftDistance = 9999999;
                    int rightDistance = 999999;
                    for(int i = 0; i < 4; i++)
                    {
                        if(i != biggestPointIndex && i != smallestPointIndex)
                        {
                            int bigDistance = cv::norm(keypoints[biggestPointIndex].pt - keypoints[i].pt);
                            int smallDistance = cv::norm(keypoints[smallestPointIndex].pt - keypoints[i].pt);
                            if(bigDistance < leftDistance)
                            {
                                topLeftPointIndex = i;
                                leftDistance = bigDistance;
                            }
                            if(smallDistance < rightDistance)
                            {
                                bottomRightIndex = i;
                                rightDistance = smallDistance;
                            }
                        }
                    }

                    double TLr = keypoints[topLeftPointIndex].size / 2;
                    QString TLText = QString("TL(S:%1)").arg(TLr *TLr * 3.14);
                    QString TRText = QString("TR(S:%1)").arg(keypoints[smallestPointIndex].size);
                    QString BLText = QString("BL(S:%1)").arg(keypoints[biggestPointIndex].size);
                    QString BRText = QString("BR(S:%1)").arg(keypoints[bottomRightIndex].size);
                    cv::putText(keypointsImage,TLText.toUtf8().constData(),keypoints[topLeftPointIndex].pt,1,1,cv::Scalar(255,0,0));
                    cv::putText(keypointsImage,TRText.toUtf8().constData(),keypoints[smallestPointIndex].pt,1,1,cv::Scalar(255,0,0));
                    cv::putText(keypointsImage,BLText.toUtf8().constData(),keypoints[biggestPointIndex].pt,1,1,cv::Scalar(255,0,0));
                    cv::putText(keypointsImage,BRText.toUtf8().constData(),keypoints[bottomRightIndex].pt,1,1,cv::Scalar(255,0,0));



                    std::vector<cv::Point2f> inputPoints, outputPoints;

                    cv::Mat contentAreaImage = cv::Mat(200,400, CV_8UC1, cv::Scalar(255,255,255));

                    inputPoints.push_back(keypoints[topLeftPointIndex].pt);
                    inputPoints.push_back(keypoints[smallestPointIndex].pt);
                    inputPoints.push_back(keypoints[bottomRightIndex].pt);
                    inputPoints.push_back(keypoints[biggestPointIndex].pt);

                    outputPoints.push_back(cv::Point2f(0,0));
                    outputPoints.push_back(cv::Point2f(contentAreaImage.cols - 1,0));
                    outputPoints.push_back(cv::Point2f(contentAreaImage.cols - 1, contentAreaImage.rows -1));
                    outputPoints.push_back(cv::Point2f(0,contentAreaImage.rows -1));


                    cv::Mat lamda = cv::getPerspectiveTransform(inputPoints, outputPoints);
                    cv::warpPerspective(grayImage,contentAreaImage,lamda,contentAreaImage.size());
                    cv::Rect textArea = cv::Rect(contentAreaImage.cols * 0.1, contentAreaImage.rows * 0.1, contentAreaImage.cols * 0.8, contentAreaImage.rows * 0.8);
                    cv::Mat textAreaImage = contentAreaImage(textArea);
                    cv::rectangle(contentAreaImage,textArea,cv::Scalar(0,0,255));


                    //cv::Mat textAreaImageGray, thresholdImage;
                    //cv::cvtColor(textAreaImage,textAreaImageGray,cv::COLOR_BGR2GRAY);
                    //cv::threshold(textAreaImageGray,thresholdImage,50,255,cv::THRESH_BINARY);

                    //cv::imshow("contentAreaImage",contentAreaImage);
                    //cv::imshow("textAreaImage",textAreaImage);
                    //cv::imshow("thresholdImage",thresholdImage);


                    _ocr->SetImage(textAreaImage.data, textAreaImage.cols, textAreaImage.rows,textAreaImage.channels(),textAreaImage.step);


                    ContentText = QString(_ocr->GetUTF8Text()).trimmed();
                    if(!ContentText.isEmpty())
                    {
                        Confidence = _ocr->MeanTextConf();
                        float y = keypoints[smallestPointIndex].pt.y - keypoints[topLeftPointIndex].pt.y;
                        float x = keypoints[topLeftPointIndex].pt.x - keypoints[smallestPointIndex].pt.x;
                        Angle = qAtan2(y,x) * 57.2958;
                        PositionX =  (keypoints[topLeftPointIndex].pt.x + keypoints[smallestPointIndex].pt.x +  keypoints[biggestPointIndex].pt.x + keypoints[bottomRightIndex].pt.x)/4;
                        PositionY =  (keypoints[topLeftPointIndex].pt.y + keypoints[smallestPointIndex].pt.y +  keypoints[biggestPointIndex].pt.y + keypoints[bottomRightIndex].pt.y)/4;
                    }
                    //_ocr->End();

                    //cv::imwrite("/home/centria/projects/Ohjelmistoprojekti/RESTAPI/Image.jpg", image);

                }

                //cv::imshow("keypointsImage", keypointsImage);
            }
            cv::drawKeypoints( grayImage, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

            // Show blobs
            //cv::imshow("keypoints", im_with_keypoints );


            cv::imshow("image",image);
            //cv::imshow("grayImage",grayImage);
            _detector->clear();
            WriteResults();
            cv::imwrite(ImageFilename.toUtf8().data(), image);
        }
        else
        {
            _videoCapture.release();
        }
    }
}

void BGBlobDetector::LoadConfigurationFile()
{
    QFile configurationFile;
    configurationFile.setFileName(ConfigurationFilename);
    configurationFile.open(QIODevice::ReadOnly);
    if(configurationFile.isOpen())
    {
        QString configurationXML = configurationFile.readAll();

        QFileInfo fileInfo(configurationFile);
        QDateTime modifiedDateTime = fileInfo.lastModified();

        configurationFile.close();
        if(modifiedDateTime != _lastModifiedDateTime)
        {
            _lastModifiedDateTime = modifiedDateTime;
            QMap<QString, QString> configurations = CentriaXMLParser::ParseXMLContent("OCR_CONFIGURATION", configurationXML);

            if(!configurations["MIN_THRESHOLD"].isEmpty())
            {
                MinThreshold = configurations["MIN_THRESHOLD"].toInt();
            }

            if(!configurations["MAX_THRESHOLD"].isEmpty())
            {
                MaxThreshold = configurations["MAX_THRESHOLD"].toInt();
            }

            if(!configurations["MIN_AREA"].isEmpty())
            {
                MinArea = configurations["MIN_AREA"].toInt();
            }

            if(!configurations["MAX_AREA"].isEmpty())
            {
                MaxArea = configurations["MAX_AREA"].toInt();
            }

            if(!configurations["MIN_CIRCULARITY"].isEmpty())
            {
                MinCircularity = configurations["MIN_CIRCULARITY"].toFloat();
            }

            if(!configurations["MAX_CIRCULARITY"].isEmpty())
            {
                MaxCircularity = configurations["MAX_CIRCULARITY"].toFloat();
            }

            if(!configurations["MIN_INERTIA"].isEmpty())
            {
                MinInertia = configurations["MIN_INERTIA"].toFloat();
            }

            if(!configurations["MAX_INERTIA"].isEmpty())
            {
                MaxInertia = configurations["MAX_INERTIA"].toFloat();
            }
        }
    }

}

void BGBlobDetector::WriteResults()
{
    QFile resultFile;
    resultFile.setFileName(ResultFilename);
    resultFile.open(QIODevice::WriteOnly);
    if(resultFile.isOpen())
    {
        QByteArray resultXML = "<OCR_RESULT>";
        resultXML.append(QString("<TEXT>%1</TEXT>").arg(ContentText));
        resultXML.append(QString("<RELIABILITY>%1</RELIABILITY>").arg(Confidence));
        resultXML.append(QString("<POSITIONX>%1</POSITIONX>").arg(PositionX));
        resultXML.append(QString("<POSITIONY>%1</POSITIONY>").arg(PositionY));
        resultXML.append(QString("<ANGLE>%1</ANGLE>").arg(Angle));
        resultXML.append("</OCR_RESULT>");
        resultFile.write(resultXML);
        resultFile.close();


    }
}



void BGBlobDetector::MouseCallBack(int event, int x, int y, int flags, void *userdata)
{

}
