#ifndef UTILS_H
#define UTILS_H

#include <QImage>
#include <QString>
#include <vector>

using namespace std;

class Utils{
  public:
    Utils();
    int WHITE  = 0;
    int BLACK  = 255;

    static QImage* loadImage(QString &path){
        QImage *img = new QImage();
        img->load(path);
        return img;
    }

    static vector<vector<int>> getEmptyDoubleVector(int width , int height){
        vector<vector<int>> matrix;
        matrix.resize(height);
        for (int i = 0 ; i < matrix.size() ; i ++){
            matrix.at(i).resize(width);
        }
        return matrix;
    }

    static int toCorrectLight(double value){
        if (value > 255) return 255;
        if(value < 0 ) return 0 ;
        return (int )value;
    }

    static int toCorrectLight(int value){
        if (value > 255) return 255;
        if(value < 0 ) return 0 ;
        return (int )value;
    }

    static int calculeNormalCoords(int value, int maxCoodrs){
        if(value > (maxCoodrs-1)) return maxCoodrs - 1;
        if(value < 0 ) return 0 ;
        else return value;
    }

    static int getPixelLight(int x, int y , QImage &image){
        return image.pixelColor(x,y).blue();
    }

    static QImage doBinary(QImage image,int treshold,bool isInverse){
        int imagePixel = isInverse?0:255;
        int noImagePixel =isInverse?255:0 ;
        for(int w = 0 ; w < image.width() ; w++ ){
            for (int h = 0 ; h < image.height(); h ++){
                if(image.pixelColor(w,h).blue() >treshold){
                    image.setPixelColor(w,h,QColor(imagePixel,imagePixel,imagePixel,255));
                }else{
                    image.setPixelColor(w,h,QColor(noImagePixel,noImagePixel,noImagePixel,255));
                }
            }
        }
        return image;
    }    

    static vector<vector<int>> prepareQImageToBitMap(QImage &image){
        vector<vector<int>> bitMap;
        resizeDoubleVector(bitMap,image.width(),image.height());

        for (int h = 0 ; h < image.height() ; h ++ ){
            for(int w = 0 ; w < image.width() ; w ++ ){
                 bitMap[h][w] = getPixelLight(w,h,image)/255;
            }
        }

        return bitMap;
    }

    static void resizeDoubleVector(vector<vector<int>> &value,int w,int h){
        value.resize(h);
        for(int y = 0 ; y < h ; y++){
            value.at(y).resize(w);
        }
    }

    static void prepareBitMapToQImage(QImage &image,vector<vector<int>> &bitMap){
        for (int h = 0 ; h < image.height() ; h ++ ){
            for(int w = 0 ; w < image.width() ; w ++ ){
                 image.setPixelColor(w,h,QColor(bitMap[h][w],bitMap[h][w],bitMap[h][w],255));
            }
        }
    }

    static bool imImage (int x, int y ,QImage &iamge){
        return (x >= 0 && y >= 0 && x < iamge.width() && y < iamge.height());
    }

};

#endif // UTILS_H
