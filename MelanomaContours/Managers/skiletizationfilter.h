#ifndef SKILETIZATIONFILTER_H
#define SKILETIZATIONFILTER_H

#include <QImage>
#include <vector>

using namespace std;

/*!
 * \brief Класс выделения линий скелета
 */
class SkiletizationFilter
{
public:
    void doFilter(QImage &image);
    SkiletizationFilter();    
private:
    vector<vector<int>> bitmap;
    vector<vector<int>> skeletonZhangSuen();
    int tla(vector<vector<int>> &image, int i, int j, vector<int> &p, int &b);
    void deleteMarked(vector<vector<int>> &markBmp, vector<vector<int>> &bitMap);
};

#endif // SKILETIZATIONFILTER_H
