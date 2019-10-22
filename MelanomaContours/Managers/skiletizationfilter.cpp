#include "skiletizationfilter.h"
#include "vector"
#include "utils.h"

using namespace  std;

SkiletizationFilter::SkiletizationFilter(){}

void SkiletizationFilter::doFilter(QImage &image)
{
    bitmap = Utils::prepareQImageToBitMap(image);
    skeletonZhangSuen();
    for (int h = 0 ; h < image.height() ; h ++ ){
        for(int w = 0 ; w < image.width() ; w ++ ){
             image.setPixelColor(w,h,QColor(bitmap[h][w]*255,bitmap[h][w]*255,bitmap[h][w]*255,255));
        }
    }
}

vector<vector<int>> SkiletizationFilter::skeletonZhangSuen(){
    int w, h, b, a, p1, p2, p3, p4;
    vector<int> p;
    p.resize(8);
    bool flag = true;
    vector<vector<int>> markMap;
    Utils::resizeDoubleVector(markMap,bitmap.at(0).size(),bitmap.size());
    int countIteration = 0;
    while (flag){
        countIteration++;
        flag = false;
        for (w = 1; w < bitmap.at(0).size() - 1; w++){
            for (h = 1; h < bitmap.size() - 1; h++){
                if (bitmap[h][w] == 0){
                    markMap[h][w] = 0;
                    continue;
                }

                a = tla(bitmap, w, h, p,b);
                p1 = p[0] * p[2] * p[4];
                p2 = p[2] * p[4] * p[6];

                if ((a == 1) && ((b >= 2) && (b <= 6)) &&
                        (p1 == 0) && (p2 == 0)){
                    markMap[h][w] = 1;
                    flag = true;
                }
                else{
                    markMap[h][w] = 0;
                }
            }
        }

        deleteMarked(markMap,bitmap);

        for (w = 1; w < bitmap.at(0).size() - 1; w++){
            for (h = 1; h < bitmap.size() - 1; h++){
                if (bitmap[h][w] == 0){
                    markMap[h][w] = 0;
                    continue;
                }
                a = tla(bitmap, w, h,p,b); 
                p1 = p[0] * p[2] * p[6];
                p2 = p[0] * p[4] * p[6];

                if ((a == 1) && ((b >= 2) && (b <= 6)) &&
                        (p1 == 0) && (p2 == 0)){
                    markMap[h][w] = 1;
                    flag = true;
                } else{
                    markMap[h][w] = 0;
                }
            }
        }
        deleteMarked(markMap,bitmap);
    }

    for (w = 1; w < bitmap.at(0).size() - 1; w ++) {
        for (h = 1; h < bitmap.size() - 1; h ++){
            if (bitmap[h][w] == 0){
                markMap[h][w] = 0;
                continue;
            }
            tla(bitmap, w, h,p,b);
            p1 = ((p[7] == 1) ? 0 : 1) * p[2] * p[4];
            p2 = ((p[3] == 1) ? 0 : 1) * p[6] * p[0];
            p3 = ((p[1] == 1) ? 0 : 1) * p[4] * p[6];
            p4 = ((p[5] == 1) ? 0 : 1) * p[0] * p[2];

            if (p1 == 1 || p2 == 1 || p3 == 1 || p4 == 1){
                bitmap[h][w] = 0;
            }
        }
    }
    return bitmap;
}

int SkiletizationFilter::tla(vector<vector<int>> &image, int w, int h,vector<int> &p, int &b){
    int m = 0;
    int n = 0;
    p[0] = image[ h - 1 ][ w ];
    p[1] = image[ h - 1 ][w + 1 ];
    p[2] = image[ h ][w + 1 ];
    p[3] = image[ h + 1][w + 1 ];
    p[4] = image[ h + 1][w ];
    p[5] = image[ h + 1][w - 1 ];
    p[6] = image[ h ][ w - 1 ];
    p[7] = image[ h - 1][w - 1 ];

    for (int k = 0 ; k < 7; k++){
        if ((p[k] == 0) && (p[k+1] == 1)){
            m++;
        }
        n += p[k];
    }

    if ((p[7] == 0) && (p[0] == 1)){
        m++;
    }
    n += p[7];
    b = n;

    return m;
}

void SkiletizationFilter::deleteMarked(vector<vector<int>> &markBmp, vector<vector<int>> &bitMap) {
   int w = bitMap.at(0).size();
   int h = bitMap.size();
   for (int i = 0; i < w; i++){
       for (int j = 0; j < h; j++){
           bitMap[j][i] -= markBmp[j][i];
       }
   }
}


