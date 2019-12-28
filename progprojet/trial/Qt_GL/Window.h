#include <QGLWidget>
#include <QTime>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QtCharts>
#include "vue_opengl.h"
#include "Systeme.h"
#include "glwidget.h"
#include <string>
#include <sstream>
#include <QColor>
#include <QValueAxis>

using namespace QtCharts;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    GLWidget * glWidget;
    QSlider * createSlider();
    QSpinBox * createSpin();
    QDoubleSpinBox * createdoubleSpin(double step = 0.01);
    QSplineSeries * createScatterSeries();
    QChart * createMyChart(QString s = "Hamiltonian");

public slots:
    void updatelabelO()
    {
        // do that at the system level
        std::string s = "B =  ";
        std::stringstream ss;
        ss << (glWidget->system_tab[0])->B << std::endl;
        ss << "Time: " << (glWidget->system_tab[0])->time;
        ss << ", dt: " << glWidget->dt << std::endl;
        ss << "Total angle: " << (glWidget->system_tab[0])->totalangle() << std::endl;
        ss << "Number of magnets in the system: " << (glWidget->system_tab[0])->tab_ptr_Magnets.size() << std::endl;
        ss << "Correlation: " << (glWidget->system_tab[0])->NearestCorrelation() << std::endl;
        ss << "Hamiltonian: " << (glWidget->system_tab[0])->Energy();
        ss << std::endl;
        ss << "Kinetic Energy: " << (glWidget->system_tab[0])->KineticEnergy;
        ss << ", Potential Energy: " << (glWidget->system_tab[0])->PotentialEnergy << std::endl;
        ss << "Friction: " << (glWidget->system_tab[0])->f;


        s += ss.str();
        QString qstr = QString::fromStdString(s);
        labelO->setText(qstr);
        double dt(glWidget->dt);
        if ((glWidget->system_tab[0])->time > 0.00
          and std::fmod((glWidget->system_tab[0])->time, 100.0 * dt) <= 10.0 * dt)
        {   if (DoOnce == 0){
                double initialMagnetLength =(glWidget->system_tab[0])->MagnetLength();
                MagnetLength ->setValue(initialMagnetLength);

                double initialMagnetCharge =(glWidget->system_tab[0])->MagnetCharge();
                MagnetStrength ->setValue(initialMagnetCharge);
                DoOnce = 1;
            }
            HamiltonianTime->append((glWidget->system_tab[0])->time, (glWidget->system_tab[0])->Energy());
            KineticTime->append((glWidget->system_tab[0])->time, (glWidget->system_tab[0])->KineticEnergy);
            PotentialTime->append((glWidget->system_tab[0])->time, (glWidget->system_tab[0])->PotentialEnergy);
            CorrelationTime->append((glWidget->system_tab[0])->time,
              (glWidget->system_tab[0])->NearestCorrelation());

            if ((glWidget->system_tab[0])->KineticEnergy < 1) { myaxis1->setMax(1); }
            else {
                myaxis1->setMax((glWidget->system_tab[0])->KineticEnergy * 1.2);
            }

            double a = (glWidget->system_tab[0])->PotentialEnergy * 1.2;
            if (a < -1) {
                myaxis1->setMin(a);
            }
            else {
                myaxis1->setMin(a - 0.5);
            }

        }

    }

    void setmagnetT(int mn)
    {
        std::string s = "";
        std::stringstream ss;
        int siz(((glWidget->system_tab[0])->tab_ptr_Magnets).size());
        ((glWidget->system_tab[0])->tab_ptr_Magnets[mn % siz])->display(ss);
        s += ss.str();
        QString qstr = QString::fromStdString(s);
        labels->setText(qstr);
    }

    void setangleT(int mn){ setmagnetT(mn); }

    void saveToFile(){
        QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Magnets' Angles"), "",
                tr("All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;}
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_4_5);
            out<<*(glWidget->system_tab[0]);
        }
    }
    void loadFromFile()
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load Magnets' Angles"), "",
            tr("All Files (*)"));
        if (fileName.isEmpty())
                return;
            else {

                QFile file(fileName);

                if (!file.open(QIODevice::ReadOnly)) {
                    QMessageBox::information(this, tr("Unable to open file"),
                        file.errorString());
                    return;
                }
            (glWidget->system_tab[0])->setangles(fileName.toUtf8().constData());
        }
    }


public:
    QSlider * xSlider;
    QSlider * ySlider;
    QSlider * zSlider;
    QLabel * labels;
    QLabel * labelO;
    QSpinBox * magnetselector;
    QDoubleSpinBox * angleselector;
    QDoubleSpinBox * xselector;
    QDoubleSpinBox * yselector;
    QDoubleSpinBox * zselector;
    QPushButton * flipangle;
    QPushButton * omegato0;
    QSplineSeries * HamiltonianTime;
    QSplineSeries * KineticTime;
    QSplineSeries * PotentialTime;
    QSplineSeries * CorrelationTime;
    QChart * chart1;
    QChartView * chartview1;
    QChart * chart2;
    QChartView * chartview2;
    QValueAxis * myaxis1;
    QValueAxis * myaxis2;
    // Dimensions, strength and MagnetType double box
    QDoubleSpinBox * MagnetStrength;
    QDoubleSpinBox * MagnetLength;
    QPushButton * MagnetsLoad; // Button to load Magnets from a text file
    QPushButton * MagnetsSave; // Button to save Magnets after simulation in a text file
    bool DoOnce;

    // output or load angles

};

// double spinbox for moment, angle (double connected)
// Qlabel for time current magnet position and mass radius, height
