#include "img_pre.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "ImageException.h"
#include "opencv2/imgproc/imgproc.hpp"
/*
* author :  Shucheng Yin
* stu_id :  SA14011016
* e-mail :  ysc6688@mail.ustc.edu.cn
*  date  :  2014-11-22
*  file  :  Image.cpp
*  dscp  :  The decfinition of class Image
*/

using namespace cv;
using namespace std;

static const int BLACK = 0;
static const int WHITE = 255;
static const int BOUNDRY_NUM = 4;
static const int EXPAND_WIDTH = 20;
static const int WIDTH = 16;
static const int HEIGHT = 16;
static const int ANGLE = 15;
static const int SCALE = 50;
static const int ORDER = 4;

Image::Image(const string pFilename){
    m_Mat = imread(pFilename);
    //m_Mat = m_Mat(cv::Range::all(), cv::Range(27, 119));
    try {
        if (m_Mat.empty())
        {
            throw LoadException(pFilename);
        }
    }
    catch (LoadException &e)
    {
        cerr << e.what() << endl;
        exit(0);
    }
}

Image::Image(const cv::Mat &pMat)
{
    m_Mat = pMat;
}

void Image::setPixel(int x, int y, uchar pValue)
{
    *(m_Mat.data + y*m_Mat.step[0] + x) = pValue;
}

int Image::LoadImg(const char *pFilename)
{
    m_Mat = imread(pFilename);
    try {
        if (m_Mat.empty())
        {
            throw LoadException(pFilename);
        }
    }
    catch (LoadException &e)
    {
        cerr << e.what() << endl;
        exit(0);
    }
    std::cout << "Loading File " << pFilename << " successfully" << std::endl;
    std::cout << "***********************************" << std::endl;
    return 1;
}

void Image::ShowInWindow(const string pWinName){
    namedWindow(pWinName);
    imshow(pWinName, m_Mat);

    waitKey();
}

void Image::SaveImg(const string pFilepath, Mat& m){
    imwrite(pFilepath, m);
    std::cout << "Savinging File " << pFilepath << " successfully" << std::endl;
}

void Image::SaveImg(const string pFilepath) {
    imwrite(pFilepath, m_Mat);
    std::cout << "Savinging File " << pFilepath << " successfully" << std::endl;
}

Mat Image::toGray(){
    Mat iGray;
    if (getChannel() == 3)
    {
        std::cout << "width : " << getWidth() << std::endl;
        std::cout << "height: " << getHeight() << std::endl;
        cvtColor(m_Mat, iGray, COLOR_BGR2GRAY);
        std::cout << "The file has been converted to gray map successfully" << std::endl;
        std::cout << "***********************************" << std::endl;
    }
    else
    {
        std::cout << "the image file is not RGB file!" << std::endl;
    }
    return iGray;
}

//k-mean method to find a threshold by itself
void Image::Binarization()
{
    int i, j, nWidth, nHeight;
    int nBack_count, nData_count;
    int nBack_sum, nData_sum;
    uchar ucThre, ucThre_new;
    int nValue;

    nWidth = getWidth();
    nHeight = getHeight();
    //initial the threshold
    ucThre = 0;
    ucThre_new = 127;

    std::cout << "Initial Threshold is :" << (int)ucThre_new << std::endl;

    std::cout << "***********************************" << std::endl;
    while (ucThre != ucThre_new) {
        nBack_sum = nData_sum = 0;
        nBack_count = nData_count = 0;

        for (j = 0; j < nHeight; ++j)
        {
            for (i = 0; i < nWidth; ++i)
            {
                nValue = getPixel(i, j);
                if (nValue > ucThre_new)
                {
                    nBack_sum += nValue;
                    nBack_count++;
                }
                else {
                    nData_sum += nValue;
                    nData_count++;
                }
            }
        }// end for i

        nBack_sum = nBack_sum / nBack_count;
        nData_sum = nData_sum / nData_count;
        ucThre = ucThre_new;
        ucThre_new = (nBack_sum + nData_sum) / 2;
    }// end while

    std::cout << "After Binarization threshold is :" << (int)ucThre_new << std::endl;

    int nBlack = 0;
    int nWhite = 0;

    for (j = 0; j < nHeight; ++j)
    {
        for (i = 0; i < nWidth; ++i)
        {
            nValue = getPixel(i, j);
            if (nValue > ucThre_new)
            {
                setPixel(i, j, (uchar)WHITE);
                nWhite++;
            }
            else {
                setPixel(i, j, BLACK);
                nBlack++;
            }
        }
    }

    //backgroud is black,swap black and white
    if (nBlack > nWhite)
    {
        for (j = 0; j < nHeight; ++j)
            for (i = 0; i < nWidth; ++i)
            {
                nValue = getPixel(i, j);
                if (!nValue)
                    setPixel(i, j, (uchar)WHITE);
                else
                    setPixel(i, j, BLACK);
            }
    }

    std::cout << "Binarization finished!" << std::endl;
}

void Image::RemoveLine(int nThsd)
{
    SaveImg("temp.jpg", m_Mat);
    IplImage *srcRGB = cvLoadImage("temp.jpg");
    IplImage *src = cvCreateImage(cvSize(srcRGB->width, srcRGB->height), IPL_DEPTH_8U, 1);

    cvCvtColor(srcRGB, src, CV_RGB2GRAY);
    IplImage *dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, src->nChannels);
    CvMemStorage *storage = cvCreateMemStorage();
    CvSeq *lines = 0;


    cvCanny(src, dst, 40, 90);

    lines = cvHoughLines2(dst
        , storage
        , CV_HOUGH_STANDARD
        , 1
        , CV_PI / 180
        , nThsd
        , 0
        , 0);
    for (int i = 0;i<MIN(lines->total, 100);i++) {
        float *line = (float*)cvGetSeqElem(lines, i);
        float rho = line[0];
        float threta = line[1];
        CvPoint pt1, pt2;
        double a = cos(threta), b = sin(threta);
        double x0 = a*rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 200 * (-b));
        pt1.y = cvRound(y0 + 200 * (a));
        pt2.x = cvRound(x0 - 200 * (-b));
        pt2.y = cvRound(y0 - 200 * (a));
        cvLine(src, pt1, pt2, cvScalar(255), 1, 8);
    }
    Mat temp = cvarrToMat(src);
    temp.copyTo(m_Mat);
    try {
        if (remove("temp.jpg"))
            throw RemoveException("temp.jpg");
    }
    catch (RemoveException &e)
    {
        cerr << e.what() << endl;
        exit(0);
    }
}

void Image::NaiveRemoveNoise(int pNum)
{
    //naive remove noise
    int i, j, m, n, nValue, nCount;
    int nWidth = getWidth();
    int nHeight = getHeight();
    //set boundry to be white
    for (i = 0; i < nWidth; ++i)
    {
        setPixel(i, 0, WHITE);
        setPixel(i, nHeight - 1, WHITE);
    }
    for (i = 0; i < nHeight; ++i)
    {
        setPixel(0, i, WHITE);
        setPixel(nWidth - 1, i, WHITE);
    }
    //if the neighbor of a point is white but it is black, delete it
    for (j = 1; j < nHeight; ++j)
        for (i = 1; i < nWidth; ++i)
        {
            nValue = getPixel(i, j);
            if (!nValue)
            {
                nCount = 0;
                for (m = i - 1; m <= i + 1; ++m)
                    for (n = j - 1; n <= j + 1; ++n)
                    {
                        if (!getPixel(m, n))
                            nCount++;
                    }
                if (nCount <= pNum)
                    setPixel(i, j, WHITE);
            }
            else
            {
                nCount = 0;
                for (m = i - 1; m <= i + 1; ++m)
                    for (n = j - 1; n <= j + 1; ++n)
                    {
                        if (!getPixel(m, n))
                            nCount++;
                    }
                if (nCount >= 7)
                    setPixel(i, j, BLACK);
            }
        }
}

void Image::ContoursRemoveNoise(Mat &pMat, double pArea)
{
    double dArea;
    Mat temp;
    CvSeq* contour = NULL;
    uchar *uc;

    //remove noise by contour-detected
    pMat.copyTo(temp);
    IplImage pTemp(temp);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvContourScanner scanner = NULL;

    scanner = cvStartFindContours(&pTemp, storage, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

    CvRect rect;
    while ((contour = cvFindNextContour(scanner)) != NULL)
    {
        dArea = fabs(cvContourArea(contour));
        rect = cvBoundingRect(contour, 0);
        if (dArea < pArea)
        {
            for (int y = rect.y;y<rect.y + rect.height;y++)
            {
                for (int x = rect.x;x<rect.x + rect.width;x++)
                {
                    uc = (uchar*)(pMat.data + pMat.step[0] * y + x);
                    *uc = WHITE;
                }
            }
        }
    }
}

void Image::ContoursRemoveNoise(double pArea)
{
    int i, j;
    int color = 1;
    int nHeight = getHeight();
    int nWidth = getWidth();

    for (i = 0; i < nWidth; ++i)
        for (j = 0; j < nHeight; ++j)
        {
            if (!getPixel(i, j))
            {
                //FloodFill each point in connect area using different color
                floodFill(m_Mat, cvPoint(i, j), cvScalar(color));
                color++;
            }
        }

    int ColorCount[255] = { 0 };
    for (i = 0; i < nWidth; ++i)
    {
        for (j = 0; j < nHeight; ++j)
        {
            //caculate the area of each area
            if (getPixel(i, j) != 255)
            {
                ColorCount[getPixel(i, j)]++;
            }
        }
    }
    //get rid of noise point
    for (i = 0; i < nWidth; ++i)
    {
        for (j = 0; j < nHeight; ++j)
        {
            if (ColorCount[getPixel(i, j)] <= pArea)
            {
                setPixel(i, j, WHITE);
            }
        }
    }
    for (i = 0; i < nWidth; ++i)
    {
        for (j = 0; j < nHeight; ++j)
        {
            if (getPixel(i, j) < WHITE)
            {
                setPixel(i, j, BLACK);
            }
        }
    }
}

vector<Image> Image::xProjectDivide(int nMin_thsd, int nMax_thsd, bool show)
{
    int i, j;
    int nWidth = getWidth();
    int nHeight = getHeight();
    int *xNum = new int[nWidth];

    //inital the x-projection-num
    memset(xNum, 0, nWidth * sizeof(int));

    //compute the black pixel num in X coordinate
    for (j = 0; j < nHeight; ++j)
        for (i = 0; i < nWidth; ++i)
        {
            if (getPixel(i, j) == BLACK) xNum[i]++;
        }
    /*-----------------show x project map-------------------*/
    Mat xProjectResult(nHeight / 2, nWidth, CV_8U, Scalar(WHITE));

    for (i = 0; i < xProjectResult.cols - 1; ++i)
    {
        int begin, end;
        if (xNum[i] > xNum[i + 1])
        {
            begin = xNum[i + 1];
            end = xNum[i];
        }
        else {
            begin = xNum[i];
            end = xNum[i + 1];
        }
        for (j = begin; j <= end; ++j)
        {
            *(xProjectResult.data + xProjectResult.step[0] * (nHeight / 2 - j - 1) + i) = BLACK;
        }
    }

    std::cout << "The porject of BLACK pixel in X coordinate is in the window" << std::endl;
    if (show) {
        namedWindow("xProjectResult");
        imshow("xProjectResult", xProjectResult);
        waitKey();
    }
    /*-----------------show x project map-------------------*/

    /*-------------------divide the map---------------------*/
    vector<int> vPoint;
    int nMin, nIndex;
    if (xNum[0] > BOUNDRY_NUM) vPoint.push_back(0);
    for (i = 1;i < nWidth - 1;)
    {
        if (xNum[i] < BOUNDRY_NUM)
        {
            i++;
            continue;
        }
        vPoint.push_back(i);
        //find minimum between the min_thsd and max_thsd
        nIndex = i + nMin_thsd;
        nMin = xNum[nIndex];
        for (j = nIndex;j<i + nMax_thsd;j++)
        {
            if (xNum[j] < nMin)
            {
                nMin = xNum[j];
                nIndex = j;
            }
        }
        vPoint.push_back(nIndex);
        i = nIndex + 1;
    }
    if (xNum[nWidth - 1] > BOUNDRY_NUM) vPoint.push_back(nWidth - 1);

    //save the divided characters in map vector
    int ch_width = nWidth / (vPoint.size() / 2) + EXPAND_WIDTH;
    vector<Image> vImage;
    for (j = 0; j < (int)vPoint.size(); j += 2)
    {
        Mat *mCharacter = new Mat(nHeight, ch_width, CV_8U, Scalar(WHITE));
        for (i = 0; i < nHeight; ++i)
            memcpy(mCharacter->data + i*ch_width + EXPAND_WIDTH / 2, m_Mat.data + i*nWidth + vPoint.at(j), vPoint.at(j + 1) - vPoint.at(j));
        Image::ContoursRemoveNoise(*mCharacter, 0.5);
        Mat *mResized = new Mat(SCALE, SCALE, CV_8U);
        resize(*mCharacter, *mResized, cv::Size(SCALE, SCALE), 0, 0, CV_INTER_AREA);
        Image iCh(*mResized);
        vImage.push_back(iCh);
        delete mCharacter;
    }
    //show divided characters
    if (show) {
        char window_name[12];
        for (i = 0; i < (int)vImage.size(); ++i)
        {
            sprintf(window_name, "Character%d", i);
            //vImage.at(i).NaiveRemoveNoise(1.0f);
            vImage.at(i).ShowInWindow(window_name);
        }
    }
    delete[]xNum;
    return vImage;
}

void Image::FloodFillDivide(std::vector<string> &pData, int nMin_area, char *pFilename, int flag)
{
    int i, j;
    int color = 1;
    int nHeight = getHeight();
    int nWidth = getWidth();

    int m, n1, n2;
    for (i = 0; i < nWidth; ++i)
        for (j = 0; j < nHeight; ++j)
        {
            if (getPixel(i, j) != WHITE)
            {
                n1 = 0;
                n2 = 0;
                for (m = j - 2; m < j + 2; ++m)
                    if (getPixel(i, m) != WHITE) n1++;
                for (m = i - 2; m < i + 2; ++m)
                    if (getPixel(m, j) != WHITE) n2++;
                if (n1<2 && n2<2)
                    setPixel(i, j, WHITE);
            }
        }

    for (i = 0; i < nWidth; ++i)
        for (j = 0; j < nHeight; ++j)
        {
            if (!getPixel(i, j))
            {
                //FloodFill each point in connect area using different color
                floodFill(m_Mat, cvPoint(i, j), cvScalar(color));
                color++;
            }
        }

    int ColorCount[256] = { 0 };
    for (i = 0; i < nWidth; ++i)
    {
        for (j = 0; j < nHeight; ++j)
        {
            //caculate the area of each area
            if (getPixel(i, j) != 255)
            {
                ColorCount[getPixel(i, j)]++;
            }
        }
    }
    //get rid of noise point
    for (i = 0; i < nWidth; ++i)
    {
        for (j = 0; j < nHeight; ++j)
        {
            if (ColorCount[getPixel(i, j)] <= nMin_area)
            {
                setPixel(i, j, WHITE);
            }
        }
    }

    int k = 1;
    int minX, minY, maxX, maxY;
    vector<Image> vImage;
    while (ColorCount[k])
    {
        if (ColorCount[k] > nMin_area)
        {
            minX = minY = 100;
            maxX = maxY = -1;
            //get the rect of each charactor
            for (i = 0; i < nWidth; ++i)
            {
                for (j = 0; j < nHeight; ++j)
                {
                    if (getPixel(i, j) == k)
                    {
                        if (i < minX)
                            minX = i;
                        else if (i > maxX)
                            maxX = i;
                        if (j < minY)
                            minY = j;
                        else if (j > maxY)
                            maxY = j;
                    }
                }
            }
            //copy to each standard mat
            Mat* ch = new Mat(HEIGHT, WIDTH, CV_8U, Scalar(WHITE));
            int m, n;
            m = (WIDTH - (maxX - minX)) / 2;
            n = (HEIGHT - (maxY - minY)) / 2;
            for (i = minX; i <= maxX; ++i)
            {
                for (j = minY; j <= maxY; ++j)
                {
                    if (getPixel(i, j) == k)
                    {
                        *(ch->data + ch->step[0] * (n + j - minY) + m + (i - minX)) = BLACK;
                    }
                }
            }
            //rotate the character
            //Image::TiltCorrect(ch);
            //Mat *mResized = new Mat(SCALE,SCALE,CV_8U);
            //resize(*ch,*mResized,cv::Size(SCALE,SCALE),0,0,CV_INTER_AREA);
            //Image iCh(*mResized);

            //rebuild the pixel value because the TillCorrect has changed some pixel value
            Image *iCh = new Image(*ch);
            int ii, jj;
            for (ii = 0; ii < iCh->getWidth(); ++ii)
                for (jj = 0; jj < iCh->getHeight(); ++jj)
                {
                    if (iCh->getPixel(ii, jj) >(WHITE) / 2)
                    {
                        iCh->setPixel(ii, jj, WHITE);
                    }
                    else
                        iCh->setPixel(ii, jj, BLACK);
                }

            vImage.push_back(*iCh);
            //delete ch;
        }
        k++;
    }
    //show the character and write the feature vector to data
    char window_name[12];
    int ii, jj, kk, nCount, x, y;
    vector<int> feature;
    char vec[5], data[80];
    for (i = 0; i < (int)vImage.size(); ++i)
    {
        sprintf(window_name, "Character%d", i);
        if (flag) 
            vImage.at(i).ShowInWindow(window_name);
        feature.clear();
        for (kk = 0; kk < SCALE; ++kk)
        {
            nCount = 0;
            x = kk / ORDER;
            y = kk % ORDER;
            for (ii = 0; ii < ORDER; ++ii)
                for (jj = 0; jj < ORDER; ++jj)
                {
                    if (vImage.at(i).getPixel(x * 4 + ii, y * 4 + jj) == BLACK)
                        nCount++;
                }
            feature.push_back(nCount);
        }
        data[0] = pFilename[i];
        data[1] = 0;
        for (kk = 0; kk < SCALE; ++kk)
        {
            sprintf(vec, ",%d", feature.at(kk));
            strcat(data, vec);
        }
        string sname(data);
        pData.push_back(sname);
    }
}

void Image::TiltCorrect()
{
    int k, i, j, min_width, nLeft, nRight, min_rotate;
    Mat rotate_mat(getHeight(), getWidth(), CV_8U, cvScalar(WHITE));
    CvPoint2D32f center;
    center.x = float(getWidth() / 2.0 + 0.5);
    center.y = float(getHeight() / 2.0 + 0.5);
    min_width = 100;
    for (k = -ANGLE; k <= ANGLE; ++k)
    {
        Mat M = getRotationMatrix2D(center, k, 1.0);
        warpAffine(m_Mat, rotate_mat, M, cvSize(getWidth(), getHeight()), INTER_LINEAR, BORDER_CONSTANT, cvScalar(WHITE));
        nLeft = 100;
        nRight = -1;
        for (i = 0; i < getWidth(); ++i)
        {
            for (j = 0; j < getHeight(); ++j)
            {
                if (*(rotate_mat.data + rotate_mat.step[0] * j + i) == BLACK)
                {
                    if (i < nLeft)
                        nLeft = i;
                    else if (i > nRight)
                        nRight = i;
                }
            }
        }
        if (nRight - nLeft < min_width) {
            min_width = nRight - nLeft;
            min_rotate = k;
        }
    }
    Mat M = getRotationMatrix2D(center, min_rotate, 1.0);
    warpAffine(m_Mat, rotate_mat, M, cvSize(getWidth(), getHeight()), INTER_LINEAR, BORDER_CONSTANT, cvScalar(WHITE));
    rotate_mat.copyTo(m_Mat);
}

void Image::TiltCorrect(Mat *pMat)
{
    int k, i, j, min_width, nLeft, nRight, min_rotate;
    Mat rotate_mat(pMat->rows, pMat->cols, CV_8U, cvScalar(WHITE));
    CvPoint2D32f center;
    center.x = float(pMat->cols / 2.0 + 0.5);
    center.y = float(pMat->rows / 2.0 + 0.5);
    min_width = 100;
    for (k = -ANGLE; k <= ANGLE; ++k)
    {
        Mat M = getRotationMatrix2D(center, k, 1.0);
        warpAffine(*pMat, rotate_mat, M, cvSize(pMat->cols, pMat->rows), INTER_LINEAR, BORDER_CONSTANT, cvScalar(WHITE));
        nLeft = 100;
        nRight = -1;
        for (i = 0; i < pMat->cols; ++i)
        {
            for (j = 0; j < pMat->rows; ++j)
            {
                if (*(rotate_mat.data + rotate_mat.step[0] * j + i) == BLACK)
                {
                    if (i < nLeft)
                        nLeft = i;
                    else if (i > nRight)
                        nRight = i;
                }
            }
        }
        if (nRight - nLeft < min_width) {
            min_width = nRight - nLeft;
            min_rotate = k;
        }
    }
    Mat M = getRotationMatrix2D(center, min_rotate, 1.0);
    warpAffine(*pMat, rotate_mat, M, cvSize(pMat->cols, pMat->rows), INTER_LINEAR, BORDER_CONSTANT, cvScalar(WHITE));
    rotate_mat.copyTo(*pMat);
}


/*
Img_pre::Img_pre(std::string p) {
    img_p = p;
    cv::Mat color_img = cv::imread(img_p);
    cv::cvtColor(color_img, image, CV_BGR2GRAY);
    image = image(cv::Range::all(), cv::Range(27, 119));
    cv::Mat src = image.clone();
    cv::GaussianBlur(src, image, cv::Size(3, 3), 0, 0);
    //std::cout << "image = " << std::endl << cv::format(image, cv::Formatter::FMT_NUMPY) << std::endl;
    //std::cout << "dims:" << color_img.dims << std::endl;
    //std::cout << "rows:" << color_img.rows << std::endl;
    //std::cout << "cols:" << color_img.cols << std::endl;
    //std::cout << "channels:" << color_img.channels() << std::endl;
    //std::cout << "type:" << color_img.type() << std::endl;
    //std::cout << "depth:" << color_img.depth() << std::endl;
    //std::cout << "elemSize:" << color_img.elemSize() << std::endl;
    //std::cout << "elemSize1:" << color_img.elemSize1() << std::endl;
    //std::cout << "dims:" << image.dims << std::endl;
    //std::cout << "rows:" << image.rows << std::endl;
    //std::cout << "cols:" << image.cols << std::endl;
    //std::cout << "channels:" << image.channels() << std::endl;
    //std::cout << "type:" << image.type() << std::endl;
    //std::cout << "depth:" << image.depth() << std::endl;
    //std::cout << "elemSize:" << image.elemSize() << std::endl;
    //std::cout << "elemSize1:" << image.elemSize1() << std::endl;
    //std::cout << "1  " << int(image.at<unsigned char>(1, 1)) << std::endl;
    //std::cout << "2  " << int(image.ptr<unsigned char>(69)[149]) << std::endl;
}


void Img_pre::show_it(cv::Mat img) {
    cv::imshow("hahaha", img);
    cv::waitKey(0);
}

int Img_pre::get_r(){
    return image.cols;
}

int Img_pre::get_c() {
    return image.rows;
}

cv::Mat Img_pre::get_mat() {
    return image;
}
*/