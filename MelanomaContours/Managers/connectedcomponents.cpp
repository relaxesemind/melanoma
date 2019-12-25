#include "connectedcomponents.h"
/*!
 * \brief РџСѓСЃС‚РѕР№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
 */
SegmentMask::SegmentMask(){}
/*!
 * \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РїРѕ РјР°СЃРєРµ Рё РІРµСЂС…РЅРµРјСѓ Р»РµРІРѕРјСѓ СѓРіР»Сѓ.
 * \param segmask - РјР°СЃРєР°
 * \param lefttop - С‚РѕС‡РєР° СЃРјРµС‰РµРЅРёСЏ
 */
SegmentMask::SegmentMask(const cv::Mat1b &segmask, cv::Point lefttop):
    _segmask(segmask), _lefttop(lefttop){}

/*!
 * \brief РќР°Р»РѕР¶РµРЅРёРµ РјР°СЃРєРё
 * \param img - РѕСЂРёРіРёРЅР°Р»СЊРЅРѕРµ РёР·РѕР±СЂР°Р¶РµРЅРёРµ
 * \return РёР·РѕР±СЂР°Р¶РµРЅРёРµ СЃРµРіРјРµРЅС‚Р° РЅР° С‡РµСЂРЅРѕРј С„РѕРЅРµ (СЂР°Р·РјРµСЂС‹ = СЂР°Р·РјРµСЂР°Рј РјР°СЃРєРё)
 */
cv::Mat SegmentMask::operator()(const cv::Mat &img)
{
    cv::Mat ret;
    cv::Rect roi = {_lefttop.x, _lefttop.y, _segmask.cols, _segmask.rows};
    img(roi).copyTo(ret, _segmask);
    return ret;
}

/*!
 * \brief РџСѓСЃС‚РѕР№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
 */
ConnectedComponents::ConnectedComponents():
    _connectivity(-1), _ccltype(-1)
{

}

/*!
 * \brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ, СЃРµРіРјРµРЅС‚РёСЂСѓСЋС‰РёР№ РёР·РѕР±СЂР°Р¶РµРЅРёРµ.
 * \details РћР±РµСЃРїРµС‡РёРІР°РµС‚ РїРѕРёСЃРє РѕРґРЅРѕС†РІРµС‚РЅС‹С… СЃРІСЏР·РЅС‹С… СЃРµРіРјРµРЅС‚РѕРІ РЅР° img1b (opencv connectedComponentsWithStats),
 * Рё СЃРѕС…СЂР°РЅРµРЅРёРµ РёРЅС„РѕСЂРјР°С†РёРё Рѕ РЅРёС…. РЎРµРіРјРµРЅС‚(С‹) СЃ СЏСЂРєРѕСЃС‚СЊСЋ 0 РІСЃРµРіРґР° РїСЂРёРЅРёРјР°РµС‚СЃСЏ Р·Р° РµРґРёРЅС‹Р№ СЃРµРіРјРµРЅС‚ С„РѕРЅР°, Р° РµРіРѕ
 * РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ = 0.
 * \param img1b - РёР·РѕР±СЂР°Р¶РµРЅРёРµ (РїРѕРґРіРѕС‚РѕРІР»РµРЅРЅРѕРµ)
 * \param connectivity - СЃРІСЏР·РЅРѕСЃС‚СЊ СЃРµРіРјРµРЅС‚РѕРІ (4 РёР»Рё 8 СЃРѕСЃРµРґРµР№)
 * \param ccltype - С‚РёРї РїРѕРёСЃРєР° РѕР±Р»Р°СЃС‚РµР№
 * (СЃРј. https://docs.opencv.org/3.4.0/d3/dc0/group__imgproc__shape.html#ga5ed7784614678adccb699c70fb841075)
 */
ConnectedComponents::ConnectedComponents(const cv::Mat1b &img1b, int connectivity, int ccltype):
    _labels(cv::Mat1i(img1b.size())), _connectivity(connectivity), _ccltype(ccltype)
{
    using namespace cv;
    Mat1i stats;
    Mat1d centroids;
    connectedComponentsWithStats(img1b, _labels, stats, centroids, _connectivity, _labels.type(), _ccltype);
    for(int label = 0; label < stats.rows; ++label)
    {
        int x, y, w, h;
        const int* ptr = stats[label];
        x = ptr[CC_STAT_LEFT];
        y = ptr[CC_STAT_TOP];
        w = ptr[CC_STAT_WIDTH];
        h = ptr[CC_STAT_HEIGHT];
        _rects[label] = Rect(x, y, w, h);
        _areas[label] = ptr[CC_STAT_AREA];
        _labels_set.insert(label);
    }
}

/*!
 * \brief РљРѕРїРёСЂСѓСЋС‰РёР№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ.
 * \param cc - РєР°СЂС‚Р° СЃРµРіРјРµРЅС‚Р°С†РёРё.
 */
ConnectedComponents::ConnectedComponents(const ConnectedComponents &cc)
{
    _labels = cc._labels;
    _rects = cc._rects;
    _areas = cc._areas;
    _labels_set = cc._labels_set;

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief РџРµСЂРµРјРµС‰Р°СЋС‰РёР№ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ.
 * \param cc - РєР°СЂС‚Р° СЃРµРіРјРµРЅС‚Р°С†РёРё
 */
ConnectedComponents::ConnectedComponents(ConnectedComponents &&cc)
{
    cv::swap(_labels, cc._labels);
    std::swap(_rects, cc._rects);
    std::swap(_areas, cc._areas);
    std::swap(_labels_set, cc._labels_set);

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief РџРµСЂРµРјРµС‰Р°СЋС‰РµРµ РїСЂРёСЃРІР°РёРІР°РЅРёРµ.
 * \param cc - РєР°СЂС‚Р° СЃРµРіРјРµРЅС‚Р°С†РёРё.
 */
void ConnectedComponents::operator=(ConnectedComponents &&cc)
{
    cv::swap(_labels, cc._labels);
    std::swap(_rects, cc._rects);
    std::swap(_areas, cc._areas);
    std::swap(_labels_set, cc._labels_set);

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief РљРѕРїРёСЂСѓСЋС‰РµРµ РїСЂРёСЃРІР°РёРІР°РЅРёРµ
 * \param cc - РєР°СЂС‚Р° СЃРµРіРјРµРЅС‚Р°С†РёРё.
 */
void ConnectedComponents::operator=(const ConnectedComponents &cc)
{
    _labels = cc._labels;
    _rects = cc._rects;
    _areas = cc._areas;
    _labels_set = cc._labels_set;

    _connectivity = cc._connectivity;
    _ccltype = cc._ccltype;
}

/*!
 * \brief РїСЂРѕРІРµСЂРєР° РЅР° РЅР°Р»РёС‡РёРµ РјРµС‚РєРё
 * \param label - РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ СЃРµРіРјРµРЅС‚Р° (РјРµС‚РєР°)
 * \return true - РµСЃР»Рё СЃРµРіРјРµРЅС‚ СЃ С‚Р°РєРѕР№ РјРµС‚РєРѕР№ СЃСѓС‰РµСЃС‚РІСѓРµС‚.
 */
bool ConnectedComponents::hasLabel(int label) const
{
    return _labels_set.find(label) != _labels_set.end();
}
/*!
 * \brief Р§РёСЃР»Рѕ СЃРµРіРјРµРЅС‚РѕРІ.
 * \return С‡РёСЃР»Рѕ СЃРµРіРјРµРЅС‚РѕРІ.
 */
int ConnectedComponents::labelCount() const
{
    return int(_labels_set.size());
}

/*!
 * \brief РћРїРёСЃС‹РІР°СЋС‰РёР№ РїСЂСЏРјРѕСѓРіРѕР»СЊРЅРёРє.
 * \param label - РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ СЃРµРіРјРµРЅС‚Р° (РјРµС‚РєР°).
 * \return РѕРїРёСЃС‹РІР°СЋС‰РёР№ СЃРµРіРјРµРЅС‚ РїСЂСЏРјРѕСѓРіРѕР»СЊРЅРёРє
 */
cv::Rect ConnectedComponents::boundingRect(int label) const
{
    /*if (!hasLabel(label)) std::abort(); */
    return _rects.at(label);
}

/*!
 * \brief РџР»РѕС‰Р°РґСЊ СЃРµРіРјРµРЅС‚Р°.
 * \param label - РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ СЃРµРіРјРµРЅС‚Р° (РјРµС‚РєР°)
 * \return РїР»РѕС‰Р°РґСЊ СЃРµРіРјРµРЅС‚Р°
 */
int ConnectedComponents::segmentArea(int label) const
{
    /*if (!hasLabel(label)) std::abort(); */
    return _areas.at(label);
}
/*!
 * \brief РџРѕРґРѕРіРЅР°РЅРЅР°СЏ РјР°СЃРєР°.
 * \details Р”Р°РЅРЅР°СЏ РјР°СЃРєР° РёРјРµРµС‚ СЂР°Р·РјРµСЂС‹ РѕС…РІР°С‚С‹РІР°СЋС‰РµРіРѕ РїСЂСЏРјРѕСѓРіРѕР»СЊРЅРёРєР° Рё
 * СЃРјРµС‰РµРЅР° РІ РµРіРѕ РІРµСЂС…РЅРёР№ Р»РµРІС‹Р№ СѓРіРѕР». Р”Р»СЏ С‚Р°РєРѕР№ РјР°СЃРєРё РЅРµ РЅСѓР¶РЅРѕ Р·Р°Р±РѕС‚РёС‚СЊСЃСЏ Рѕ РЅР°СЃС‚СЂРѕР№РєРµ Р·РѕРЅС‹ РёРЅС‚РµСЂРµСЃР° -
 * Р±СѓРґСѓС‡Рё РЅР°Р»РѕР¶РµРЅРЅРѕР№ РЅР° РѕСЂРёРіРёРЅР°Р»СЊРЅРѕРµ РёР·РѕР±СЂР°Р¶РµРЅРёРµ РѕРЅР° РІРµСЂРЅРµС‚ РЅСѓР¶РЅС‹Р№ СЃРµРіРјРµС‚ РЅР° С‡РµСЂРЅРѕРј С„РѕРЅРµ, Р±РµР·
 * Р»РёС€РЅРёС… С‡РµСЂРЅС‹С… РєСЂР°РµРІ РєР°Рє РІ simpleMask.
 * * РџСЂРёРјРµСЂ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ
 * \code
    cv::threshold(src, ret, 0 ,255, THRESH_BINARY_INV | THRESH_OTSU);
    ConnectedComponents cc(ret);
    if (cc.hasLabel(1))
        cv::imshow(cc.fittedMask(1)(src));
 * \endcode
 * \param label - РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ СЃРµРіРјРµРЅС‚Р° (РјРµС‚РєР°).
 * \return
 */
SegmentMask ConnectedComponents::fittedMask(int label) const
{
    /*if (!hasLabel(label)) std::abort(); */
    using namespace cv;
    Rect bounding_rect = boundingRect(label);
    Mat1i mask = _labels(bounding_rect);
    auto lambda = [&](int& pix, const int*)
        {pix = int(pix == label);};
    mask.forEach(lambda);
    return SegmentMask(mask, bounding_rect.tl());
}

/*!
 * \brief РџСЂРѕСЃС‚Р°СЏ РјР°СЃРєР°.
 * \details РџСЂРё РЅР°Р»РѕР¶РµРЅРёРё РїСЂРѕСЃС‚РѕР№ РјР°СЃРєРё СЂР°Р·РјРµСЂ РёР·РѕР±СЂР°Р¶РµРЅРёСЏ СЃРѕС…СЂР°РЅРёС‚СЃСЏ,
 * РѕСЃС‚Р°РІР»СЏСЏ РјРЅРѕРіРѕ Р»РёС€РЅРёС… С‡РµСЂРЅС‹С… РїРёРєСЃРµР»РµР№, РІ РѕС‚Р»РёС‡Р°Рµ РѕС‚ fittedMask.
 * \param label - РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ СЃРµРіРјРµРЅС‚Р° (РјРµС‚РєР°).
 * \return
 */
SegmentMask ConnectedComponents::simpleMask(int label) const
{
    using namespace cv;
    Mat1i mask(_labels);
    auto lambda = [&](int& pix, const int*)
        {pix = int(pix == label);};
    mask.forEach(lambda);
    return SegmentMask(mask, {0,0});
}

/*!
 * \brief РњРµС‚РєР° СЃРµРіРјРµРЅС‚Р°.
 * \details Р”Р»СЏ СѓРєР°Р·Р°РЅРЅРѕР№ С‚РѕС‡РєРё pos РІРѕРІСЂР°С‰Р°РµС‚ РјРµС‚РєСѓ СЃРµРіРјРµРЅС‚Р°, РєРѕС‚РѕСЂРѕРёСѓ РѕРЅР° РїСЂРёРЅР°РґР»РµР¶РёС‚.
 * \param pos - С‚РѕС‡РєР° РёР·РѕР±СЂР°Р¶РµРЅРёСЏ
 * \return РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ СЃРµРіРјРµРЅС‚Р° (РјРµС‚РєР°)
 */
int ConnectedComponents::label(cv::Point pos) const
{
    return _labels(pos);
}

/*!
 * \brief РџРµСЂРµС‡РµРЅСЊ РґРѕСЃС‚СѓРїРЅС‹С… СЃРµРіРјРµРЅС‚РѕРІ.
 * \details Р’РѕР·РІСЂР°С‰Р°РµС‚ РјРЅРѕР¶РµСЃС‚РІРѕ РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂРѕРІ РІСЃРµС… СЃСѓС‰РµСЃС‚РІСѓСЋС‰РёС… СЃРµРіРјРµРЅС‚РѕРІ.
 * \warning РјРµС‚РѕРґС‹ join, joinToEnviroment Рё combine РјРµРЅСЏСЋС‚ СЌС‚РѕС‚ СЃРїРёСЃРѕРє.
 * РџРѕСЌС‚РѕРјСѓ РїСЂРё РѕР±С…РѕРґРµ СЃРµРіРјРµРЅС‚РѕРІ СЃ РґР°РЅРЅС‹РјРё РјРµС‚РѕРґР°РјРё РјРѕРіСѓС‚ РІРѕР·РЅРёРєР°С‚СЊ РѕС€РёР±РєРё.
 * \return
 */
const std::set<int> &ConnectedComponents::labelsSet() const
{
    return _labels_set;
}

/*!
 * \brief Р РµР№С‚РёРЅРі РїР»РѕС‰Р°РґРµР№ СЃРµРіРјРµРЅС‚РѕРІ.
 * \details РЎРѕСЃС‚Р°РІР»СЏРµС‚ СЃР»РѕРІР°СЂСЊ СЃ РїР»РѕС‰Р°РґСЏРјРё С‡Р°СЃС‚РµР№ СЃРµРіРјРµРЅС‚РѕРІ, РїРѕРїР°РґР°СЋС‰РёС… РІ roi.
 * РџРѕ СѓРјРѕР»С‡Р°РЅРёСЋ РІРµСЂРЅРµС‚ С‚Р°РєРѕР№ СЃР»РѕРІР°СЂСЊ РґР»СЏ РІСЃРµР№ РєР°СЂС‚С‹.
 * \param roi - РѕС…РІР°С‚С‹РІР°СЋС‰РёР№ РїСЂСЏРјРѕСѓРіРѕР»СЊРЅРёРє.
 * \return Р’РѕР·РІСЂР°С‰Р°РµС‚ СЃР»РѕРІР°СЂСЊ<РјРµС‚РєР°, РїР»РѕС‰Р°РґСЊ>
 */
std::map<int, int> ConnectedComponents::areas(const cv::Rect roi) const
{
    if (roi == cv::Rect()) return _areas;
    std::map<int, int> roi_areas;
    cv::Mat1i img_roi = _labels(roi);
    for(int row = 0; row < img_roi.rows; ++row)
        for(int col = 0; col < img_roi.cols; ++col)
        {
            int label = img_roi(row,col);
            roi_areas[label] += 1;
        }
    return roi_areas;
}
/*
cv::Mat1i ConnectedComponents::adjacencyMatrix() const
{
    using namespace cv;
    Mat1i AdjMat = Mat1i::zeros(labelCount(),labelCount());
    _labels.forEach([&](int& pix, const int* pos)
    {
        int y = pos[0], x = pos[1];
        for (int ny=-1;ny<=1;++ny)
            for (int nx=-1;nx<=1;++nx)
            {
                Point n{x+nx, y+ny};
                if (Rect({0,0},size()).contains(n) && (pix !=_labels(n)))
                    ++AdjMat(pix,_labels(n));
            }
    });
    return AdjMat;
}*/

/*!
 * \brief Р“СЂР°РЅРёС†Р° СЃРµРіРјРµРЅС‚Р°.
 * \details Р’РѕР·РІСЂР°С‰Р°РµС‚ СЃР»РѕРІР°СЂСЊ <РјРµС‚РєР° СЃРѕСЃРµРґР°, РјРЅРѕР¶РµСЃС‚РІРѕ С‚РѕС‡РµРє РєРѕРЅС‚СѓСЂР°> РґР»СЏ РґР°РЅРЅРѕРіРѕ СЃРµРіРјРµРЅС‚Р°.
 * \param label - РјРµС‚РєР° СЃРµРіРјРµРЅС‚Р°.
 * \return СЃР»РѕРІР°СЂСЊ <РјРµС‚РєР° СЃРѕСЃРµРґР°, РјРЅРѕР¶РµСЃС‚РІРѕ С‚РѕС‡РµРє РєРѕРЅС‚СѓСЂР°>
 */
std::map<int, std::set<cv::Point> > ConnectedComponents::segmentBorder(int label) const
{
    using namespace std;
    using namespace cv;

    Rect seg_rect = boundingRect(label);
    Rect img_rect = Rect({0,0}, size());

    map<int,set<Point>> borders;

    for(int y = seg_rect.y - 1; y <= seg_rect.br().y + 1; ++y)
        for(int x = seg_rect.x - 1; x <= seg_rect.br().x + 1; ++x)
            if(img_rect.contains({x, y}) && (_labels({x, y}) == label))
                for (int j = -1; j <= 1; ++j)
                    for (int i = -1; i <= 1; ++i)
                    {
                        if(img_rect.contains({x + i, y + j}))
                        {
                            int border_label = _labels({x + i, y + j});
                            if (border_label != label)
                                borders[border_label].insert({x/* + i*/, y/* + j*/});
                        }
                    }
    return borders;
}
/*
 * \brief segmentGraphLinkList - РјРµС‚РѕРґ, РІРѕР·РІСЂР°С‰Р°СЋС‰РёР№ РіСЂР°С„, РїСЂРµРґСЃС‚Р°РІР»РµРЅРЅС‹Р№ РІ
 * РІРёРґРµ СЃР»РѕРІР°СЂСЏ СЃРІСЏР·РµР№ РјРµР¶РґСѓ СЃРµРіРјРµРЅС‚Р°РјРё. РўР°РєРёРј РѕР±СЂР°Р·РѕРј РєР»СЋС‡ Vec2i РѕС‚СЂР°Р¶Р°РµС‚ РЅР°Р»РёС‡РёРµ
 * СЃРІСЏР·Рё (РѕР±С‰РµР№ РіСЂР°РЅРёС†С‹) РјРµР¶РґСѓ 2РјСЏ СЃРµРіРјРµРЅС‚Р°РјРё, РїСЂРё СЌС‚РѕРј РЅР° РїРµСЂРІРѕРј РјРµСЃС‚Рµ СЃРµРіРјРµРЅС‚ СЃ
 * РјРµРЅСЊС€РµР№ РјРµС‚РєРѕР№. Р—РЅР°С‡РµРЅРёРµ int - РјРѕС‰РЅРѕСЃС‚СЊ (РґР»РёРЅР°) РіСЂР°РЅРёС†С‹.
 * \return
 * \badcode
std::map<cv::Vec2i, int> ConnectedComponents::segmentGraphLinkList() const
{
    using namespace std;
    using namespace cv;

    vector<vector<Point>> contours;
    findContours(_labels,contours,noArray(),RETR_LIST,CHAIN_APPROX_NONE);

    Rect imrect({0,0}, _labels.size());
    map<Vec2i, int> links;

    for(const vector<Point>& contour: contours)
    {
        int label = _labels(contour[0]);

        for(Point p: contour)
            for(int ny=-1; ny<=1;++ny) for(int nx=-1; nx<=1;++nx)
            {
                Point np(p.x+nx,p.y+ny);
                if(imrect.contains(np) && _labels(np)!=label)
                {
                    int nlabel = _labels(np);
                    Vec2i link = (label<nlabel)?Vec2i{label,nlabel}:Vec2i{nlabel,label};
                    links[link]+=1;
                    nx = ny = 1;//break
                }
            }
    }
    return links;
}
 */
/*!
 * \brief РћР±СЉРµРґРёРЅРёС‚СЊ СЃРµРіРјРµРЅС‚С‹.
 * \details РћР±СЉРµРґРёРЅСЏРµС‚ СЃРµРіРјРµРЅС‚С‹ (РЅРµ РѕР±СЏР·Р°С‚РµР»СЊРЅРѕ СЃРІСЏР·РЅС‹Рµ) РІ РѕРґРёРЅ СЃ РјРµС‚РєРѕР№ label1.
 * РџСЂРё СЌС‚РѕРј label2 СЃС‚Р°РЅРѕРІРёС‚СЃСЏ РЅРµ РґРѕСЃС‚СѓРїРЅРѕР№.
 * \warning РњРµРЅСЏРµС‚ СЃРїРёСЃРѕРє РґРѕСЃС‚СѓРїРЅС‹С… СЃРµРіРјРµРЅС‚РѕРІ.
 * \param label1 - РјРµС‚РєР° СЃРµРіРјРµРЅС‚Р°
 * \param label2 - РјРµС‚РєР° РїСЂРёСЃРѕРµРґРёРЅСЏРµРјРѕРіРѕ СЃРµРіРјРµРЅС‚Р°
 */
void ConnectedComponents::join(int label1, int label2)
{
    using namespace cv;
    if (label1 == label2)
        throw std::invalid_argument("ConnectedComponents::join: label1 must not be equal label2");

    Rect rect1 = boundingRect(label1);
    int area1 = segmentArea(label1);

    Rect rect2 = boundingRect(label2);
    int area2 = segmentArea(label2);

    for (int y = rect2.y; y < rect2.br().y; ++y)
        for (int x = rect2.x; x < rect2.br().x; ++x)
        {
            int& pix = _labels({x, y});
            if (pix == label2) pix=label1;
        }

    Rect joined_rect = rect1 | rect2;
    _rects.at(label1) = joined_rect;
    _rects.erase(label2);

    int joined_area = area1 + area2;
    _areas.at(label1) = joined_area;
    _areas.erase(label2);
    _labels_set.erase(label2);
}

/*!
 * \brief РџСЂРёСЃРѕРµРґРёРЅРёС‚СЊ Рє РѕРєСЂСѓР¶РµРЅРёСЋ.
 * \details РџСЂРёСЃРѕРµРґРёРЅСЏРµС‚ СЃРµРіРјРµРЅС‚ СЃ РјРµС‚РєРѕР№ label Рє СЃРѕСЃРµРґСѓ СЃ РјР°РєСЃРёРјР°Р»СЊРЅРѕР№ РґР»РёРЅРЅРѕР№ РѕР±С‰РµР№ РіСЂР°РЅРёС†С‹, РїРѕСЃР»Рµ
 * С‡РµРіРѕ СЃС‚Р°РЅРѕРІРёС‚СЃСЏ РЅРµ РґРѕСЃС‚СѓРїРЅС‹Рј.
 * \warning РѕС‚РґРµР»СЊРЅС‹Рµ СЃРµРіРјРµРЅС‚С‹ С„РѕРЅР° (РјРµР»РєРёРµ РѕСЃС‚СЂРѕРІРєРё РЅСѓР»РµР№) РЅРµ Р±СѓРґСѓС‚ СѓРґР°Р»РµРЅС‹ РїРѕРґРѕР±РЅС‹Рј РјРµС‚РѕРґРѕРј.
 * \param label - РјРµС‚РєР° СЃРµРіРјРµРЅС‚Р°
 * \return РјРµС‚РєР° СЃРµРіРјРµРЅС‚Р°, Рє РєРѕС‚РѕСЂРѕРјСѓ Р±С‹Р» РїСЂРёСЃРѕРµРґРёРЅРµРЅ СЃРµРіРјРµРЅС‚.
 */
int ConnectedComponents::joinToEnviroment(int label)
{
    using namespace std;
    using namespace cv;

    auto borders = segmentBorder(label);

    int new_label = 0;
    size_t max_length = 0;
    for(const pair<int, set<Point>>& b: borders)
        if (b.second.size() > max_length)
        {
            new_label = b.first;
            max_length = b.second.size();
        }

    join(new_label, label);
    return new_label;
}
/*!
 * \brief РћР±СЉРµРґРёРЅРёС‚СЊ РјРµР»РєРёРµ РІ РєСЂСѓРїРЅС‹Рµ.
 * \details РџСЂРёСЃРѕРµРґРёРЅСЏРµС‚ РІСЃРµ РјРµР»РєРёРµ СЃРІСЏР·РЅС‹Рµ СЃРµРіРјРµРЅС‚С‹ (РІ С‚РѕРј С‡РёСЃР»Рµ СЃРІСЏР·РЅС‹Рµ СЃРµРіРјРµРЅС‚С‹ С„РѕРЅР°),
 * Рє РѕРєСЂСѓР¶Р°СЋС‰РёРј РёС… РєСЂСѓРїРЅС‹Рј, РµСЃР»Рё РёС… РїР»РѕС‰Р°РґСЊ РјРµРЅСЊС€Рµ area_threshold.
 * \warning РџСЂРё С‡РёСЃР»Рµ РєСЂСѓРїРЅС‹С… СЃРµРіРјРµРЅС‚РѕРІ РїРѕСЃР»Рµ РѕР±СЂР°Р±РѕС‚РєРё > 255 РІРѕР·РјРѕР¶РЅРѕ РёС… СЃРїР°РЅС‚Р°РЅРЅРѕРµ РѕР±СЉРµРґРёРЅРµРЅРёРµ.
 * \param area_threshold - РјРёРЅРёРјР°Р»СЊРЅР°СЏ РїР»РѕС‰Р°РґСЊ СЃРµРіРјРµРЅС‚Р°
 */
void ConnectedComponents::combine(int area_threshold)
{
    /*
    list<int> combine_list;
    for (auto p: _areas)
    {
        int area = p.second;
        if (area < area_threshold)
            combine_list.push_back(p.first);
    }
    for (int id: combine_list)
        joinToEnviroment(id);*/

    using namespace cv;
    using namespace std;
    if (area_threshold < 1) return;
    vector<vector<Point>> contours;
    findContours(_labels, contours, noArray(), CV_RETR_FLOODFILL,CHAIN_APPROX_NONE);
    multimap<double, vector<Point>> contour_map;
    for(auto c: contours)
    {
        double area = contourArea(c);
        if (area >= area_threshold)
            contour_map.insert(make_pair(area,c));
    }

    Mat1i dst = Mat1i::zeros(_labels.rows, _labels.cols);
    int color = 1;
    for(auto It = contour_map.rbegin(); It != contour_map.rend(); ++It)
    {
        vector<vector<Point>> contour = {It->second};
        if(_labels(contour[0][0]) != 0)
            drawContours( dst, contour, 0, color++, FILLED, _connectivity);
        else
            drawContours( dst, contour, 0, 0, FILLED, _connectivity);
    }
    *this = ConnectedComponents(dst, _connectivity, _ccltype);//Warning dst not Mat1b!
}

/*!
 * \brief Р Р°Р·РјРµСЂ РєР°СЂС‚С‹ СЃРµРіРјРµРЅС‚Р°С†РёРё.
 * \return С€РёСЂРёРЅР° Рё РІС‹СЃРѕС‚Р°.
 */
cv::Size ConnectedComponents::size() const
{
    return _labels.size();
}

/*!
 * \brief РћС‚СЂРёСЃРѕРІРєР° РєР°СЂС‚С‹ СЃРµРіРјРµРЅС‚Р°С†РёРё.
 * \details Р”Р»СЏ РєР°Р¶РґРѕРіРѕ РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂР° СЃРµРіРјРµРЅС‚Р° СЃР»СѓС‡Р°Р№РЅС‹Рј РѕР±СЂР°Р·РѕРј РіРµРЅРµСЂРёСЂСѓРµС‚СЃСЏ С†РІРµС‚,
 * РїРѕСЃР»Рµ С‡РµРіРѕ РѕС‚СЂРёСЃРѕРІС‹РІР°РµС‚ РєР°СЂС‚Сѓ, РІ РІРёРґРµ Р·Р°Р»РёС‚С‹С… РґР°РЅРЅС‹РјРё С†РІРµС‚Р°РјРё РѕР±Р»Р°СЃС‚РµР№.
 * \return
 */
cv::Mat3b ConnectedComponents::visualize() const
{
    using namespace std;
    using namespace cv;

    //РїРµСЂРµРїРёСЃР°С‚СЊ С‡РµСЂРµР· findContours(_labels,contours,noArray(),RETR_FLOODFILL,CHAIN_APPROX_NONE);
    // for(int i=0; i<contours.size(); ++i)
    //      drawContours(img, contours, i, randColor());


    map<int,Vec3b> colors;
    int width = _labels.cols;
    int height = _labels.rows;

    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x)
        {
            int pix = _labels({x, y});
            if (colors.find(pix) == colors.end())
                colors[pix] = {uchar(rand()%256), uchar(rand()%256), uchar(rand()%256)};
        }

    Mat3b ret(_labels.size());
    ret.forEach([&](Vec3b& pix, const int* pos)
    {
        int seg_id = _labels(pos);
        pix = colors[seg_id];
    });
    return ret;
}

/*!
 * \brief РќР°СЂРёСЃРѕРІР°С‚СЊ РіСЂР°РЅРёС†С‹.
 * \details РђРЅР°Р»РѕРіРёС‡РµРЅ visualize, РѕРґРЅР°РєРѕ РѕРЅ Р»РёС€СЊ РЅР°РєР»Р°РґС‹РІР°РµС‚ СѓРЅРёРєР°Р»СЊРЅС‹Рµ РїРѕ С†РІРµС‚Сѓ РґР»СЏ РєР°Р¶РґРѕРіРѕ СЃРµРіРјРµРЅС‚Р° РіСЂР°РЅРёС†С‹
 * СЃ С‚РѕР»С‰РёРЅРѕР№ thickness РІ РїРёРєСЃРµР»СЏС…. РџРѕСЌС‚РѕРјСѓ РєР°Р¶РґР°СЏ РіСЂР°РЅРёС†Р° РјРµР¶РґСѓ СЃРµРіРјРµРЅС‚Р°РјРё СЏРІР»СЏРµС‚СЃСЏ РґРІСѓС†РІРµС‚РЅРѕР№.
 * \param image
 * \param thickness
 */
void ConnectedComponents::drawBorders(cv::Mat3b &image, int thickness) const
{
    using namespace std;
    using namespace cv;
    int t = thickness;
    int width = image.cols, height = image.rows;
    Mat3b  visulization = visualize();
    visulization.forEach([&](Vec3b& pix, const int* pos)
    {
        int y = pos[0], x = pos[1];
        for (int ny = -t; ny <= t; ++ny)
            for (int nx =- t; nx <= t; ++nx)
            {
                Point n{x+nx, y+ny};
                if (!(0 <= n.x && n.x < width && 0 < n.y && n.y < height) || pix != visulization(n))
                {
                    image(pos) = pix;
                    nx = ny = t;//break;
                }
            }
    });
    return;
}
