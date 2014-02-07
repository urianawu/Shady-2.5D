#ifndef IMAGESHAPE_H
#define IMAGESHAPE_H

#include "./Shape.h"
#include "./customdialog.h"

#include <QString>
#include <qgl.h>

class ImageShape;
class ImageShapeCustomDialog : public CustomDialog
{
    Q_OBJECT
public:
    ImageShapeCustomDialog(ImageShape* imgS, QString title, QWidget *parent = 0, char* execLabel = 0, void (*callback)()=0, bool * ischeck = 0);
    void Initialize();
    void SetNewSize(double w, double h);

private slots:
    void LoadTextureImage(int );
    void LoadTextureImage();

private:
    ImageShape*     m_imgShape;
    QDoubleSpinBox* m_returnWidth;
    QDoubleSpinBox* m_returnHeight;
};

class ImageShape : public Shape
{

protected:

    void onApplyT(const Matrix3x3& tM)
    {
        Vec3 v   = tM*Vec3(m_width, m_height, 0);
        m_width  = v.x;
        m_height = v.y;

        v = tM*Vec3(0, 0, 1);
        pP()->set(P() + Point(v.x/v.z, v.y/v.z));
    }

public:

    ImageShape(int w = 1.0, int h = 1.0);
    ~ImageShape();
    void render(int mode);
    void calAverageNormal();

    void InitializeTex();
    void LoadTextureImage(int );
    void LoadTextureImage();
    void SetPenal(ImageShapeCustomDialog *penal){m_penal = penal;}
    float CapValue(float in_num, float low_cap, float high_cap);

    void getBBox(BBox& bbox) const;

    ImageShapeCustomDialog* GetPenal(){return m_penal;}

    enum{NO_UPDATE = 0, UPDATE_SM = 1, UPDATE_BRIGHT = 2, UPDATE_DARK = 4} texType;

    double m_alpha_th;
    double m_stretch;
    double m_assignedDepth;

    double m_width;
    double m_height;
    int m_texUpdate;
    bool m_shadowCreator;
    GLuint m_texSM;
    GLuint m_texDark;
    GLuint m_texBright;
    QString m_SMFile;
    QString m_DarkFile;
    QString m_BrightFile;
    //shape map image will be saved for further use
    QImage m_SMimg;
    int m_curTexture;
    ImageShapeCustomDialog *m_penal;
};

#endif // IMAGESHAPE_H