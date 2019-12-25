#ifndef CONNECTEDCOMPONENTS_H
#define CONNECTEDCOMPONENTS_H

#include <opencv2/opencv.hpp>

namespace std
{
/*! \ingroup Imaging
 * \brief РЎРїРµС†РёР°Р»РёР·Р°С†РёСЏ С€Р°Р±Р»РѕРЅР° less РґР»СЏ cv::Vec<_Tp1, cn>.
 * \details РџРѕР·РІРѕР»СЏРµС‚ СЃСЂР°РІРЅРёРІР°С‚СЊ 2 РІРµРєС‚РѕСЂР° РїРѕ СЂРµРєСѓСЂРµРЅС‚РЅРѕРјСѓ РїСЂР°РІРёР»Сѓ:
 *  Р•РЎР›Р lhs[i] != rhs[i] РўРћ Р’Р•Р РќРЈРўР¬ lhs[i]<rhs[i]
 *  РРќРђР§Р• РџРћР’РўРћР РРўР¬ Р”Р›РЇ i+1
 * РџРѕР·РІРѕР»СЏРµС‚ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ cv::Vec<_Tp1, cn> РєР°Рє РєР»СЋС‡ РІ СЃР»РѕРІР°СЂСЏС…, РґРµСЂРµРІСЊСЏС… Рё С‚.Рґ.
 */
template <typename _Tp1, int cn >
struct less<cv::Vec<_Tp1, cn>>
{
    bool operator()(const cv::Vec<_Tp1, cn>& lhs, const cv::Vec<_Tp1, cn>& rhs ) const
    {
        int lsize = sizeof(lhs) / sizeof(lhs[0]);
        for (int i=0; i < lsize; ++i)
            if (lhs[i] != rhs[i])
                return lhs[i] < rhs[i];
        return false;
    }
};
/*! \ingroup Imaging
 * \brief РЎРїРµС†РёР°Р»РёР·Р°С†РёСЏ С€Р°Р±Р»РѕРЅР° less РґР»СЏ cv::Point_<_Tp1>.
 * \details РџРѕР·РІРѕР»СЏРµС‚ СЃСЂР°РІРЅРёРІР°С‚СЊ 2 С‚РѕС‡РєРё РїРѕ РїСЂР°РІРёР»Сѓ:
 *  Р•РЎР›Р lhs.y!=rhs.y РўРћ Р’Р•Р РќРЈРўР¬ lhs.y<rhs.y РРќРђР§Р• Р’Р•Р РќРЈРўР¬ lhs.x<rhs.x
 * РџРѕР·РІРѕР»СЏРµС‚ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ cv::Point_<_Tp1> РєР°Рє РєР»СЋС‡ РІ СЃР»РѕРІР°СЂСЏС…, РґРµСЂРµРІСЊСЏС… Рё С‚.Рґ.
 */
template <typename _Tp1>
struct less<cv::Point_<_Tp1>>
{
    bool operator()(const cv::Point_<_Tp1>& lhs, const cv::Point_<_Tp1>& rhs ) const
    {
        if (lhs.y != rhs.y)
            return lhs.y < rhs.y;
        return lhs.x < rhs.x;
    }
};
}

/*! \ingroup Imaging
 * \brief РљР»Р°СЃСЃ РјР°СЃРєРё СЃРµРіРјРµРЅС‚Р°.
 * \details РћР±РµСЃРїРµС‡РёРІР°РµС‚ РјР°СЃРєРёСЂРѕРІР°РЅРёРµ РёРЅС‚РµСЂРµСЃСѓСЋС‰РµРіРѕ СЃРµРіРјРµРЅС‚Р°
 * (РїРѕР»СѓС‡РµРЅРёРµ РјРЅРѕР¶РµСЃС‚РІР° РїРёРєСЃРµР»РµР№ СЃРµРіРјРµРЅС‚Р° РЅР° С‡РµСЂРЅРѕРј С„РѕРЅРµ). РџРѕ
 * СЃСѓС‚Рё РЅР°РєР»Р°РґС‹РІР°РµС‚ segmask РІ С‚РѕС‡РєРµ lefttop, СѓРјРµРЅСЊС€Р°СЏ РІС‹С…РѕРґРЅСѓСЋ
 * РјР°С‚СЂРёС†Сѓ РґРѕ СЂР°Р·РјРµСЂРѕРІ segmask.
 */
class SegmentMask
{
    cv::Mat1b _segmask;
    cv::Point _lefttop;
public:
    SegmentMask();
    SegmentMask(const cv::Mat1b& segmask, cv::Point lefttop = {0,0});
    cv::Mat operator()(const cv::Mat& img);
};

/*! \ingroup Imaging
 * \brief РљР»Р°СЃСЃ РєР°СЂС‚С‹ СЃРµРіРјРµРЅС‚Р°С†РёРё.
 * \details РЇРІР»СЏРµС‚СЃСЏ СѓРґРѕР±РЅРѕР№ РѕР±РµСЂС‚РєРѕР№ РґР»СЏ РѕРґРЅРѕРёРјРµРЅРЅРѕР№
 * С„СѓРЅРєС†РёРё opencv. РћР±РµСЃРїРµС‡РёРІР°РµС‚ РїРѕРёСЃРє, С…СЂР°РЅРµРЅРёРµ, РѕР±СЉРµРґРёРЅРµРЅРёРµ СЃРµРіРјРµРЅС‚РѕРІ,
 * РіРµРЅРµСЂРёСЂРѕРІР°РЅРёСЏ РјР°СЃРѕРє SegmentMask, РІС‹С‡РёСЃР»РµРЅРёРµ РѕРїРёСЃС‹РІР°СЋС‰РёС… РїСЂСЏРјРѕСѓРіРѕР»СЊРЅРёРєРѕРІ Рё
 * РїР»РѕС‰Р°РґРµР№ СЃРµРіРјРµРЅС‚РѕРІ.
 */
class ConnectedComponents
{
private:
    cv::Mat1i _labels;
    std::map<int, cv::Rect> _rects;
    std::map<int, int> _areas;
    std::set<int> _labels_set;

    int _connectivity;
    int _ccltype;

public:
    ConnectedComponents();
    ConnectedComponents(const cv::Mat1b& img1b, int connectivity = 8, int ccltype = cv::CCL_WU);
    ConnectedComponents(const ConnectedComponents& cc);
    ConnectedComponents(ConnectedComponents&& cc);
    void operator=(const ConnectedComponents& cc);
    void operator=(ConnectedComponents&& cc);

    bool hasLabel(int label) const;
    int labelCount() const;
    cv::Rect boundingRect(int label) const;
    int segmentArea(int label) const;
    SegmentMask fittedMask(int label) const;
    SegmentMask simpleMask(int label) const;
    int label(cv::Point pos) const;
    const std::set<int> &labelsSet() const;
    std::map<int, int> areas(const cv::Rect roi = cv::Rect()) const;

    std::map<int,std::set<cv::Point>> segmentBorder(int label) const;
    //std::map<cv::Vec2i, int> segmentGraphLinkList() const;

    void join(int label1, int label2);
    int joinToEnviroment(int label);
    void combine(int area_threshold);

    cv::Size size() const;
    cv::Mat3b visualize() const;
    void drawBorders(cv::Mat3b &image, int thickness) const;
};


#endif // CONNECTEDCOMPONENTS_H
