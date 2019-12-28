#include "Window.h"
#include <string>
#include <sstream>


Window::Window()

{
    glWidget = new GLWidget;
    glWidget->setFocusPolicy(Qt::ClickFocus);
    labelO = new QLabel(this);
    updatelabelO();
    // H= blabla, correlation, time
    connect(glWidget, SIGNAL(XBChanged(double)), this, SLOT(updatelabelO()));
    connect(glWidget, SIGNAL(YBChanged(double)), this, SLOT(updatelabelO()));
    connect(glWidget, SIGNAL(ZBChanged(double)), this, SLOT(updatelabelO()));
    connect(glWidget, SIGNAL(evolved(double)), this, SLOT(updatelabelO()));
    DoOnce = 0;


    // chart1
    myaxis1         = new QValueAxis();
    chart1          = createMyChart("Energy (x10^4)");
    HamiltonianTime = createScatterSeries();
    KineticTime     = createScatterSeries();
    PotentialTime   = createScatterSeries();
    HamiltonianTime->setName("Total");
    KineticTime->setName("Kinetic");
    PotentialTime->setName("Potential");
    chart1->addSeries(HamiltonianTime);
    chart1->addSeries(KineticTime);
    chart1->addSeries(PotentialTime);
    chartview1 = new QChartView(chart1);
    chart1->createDefaultAxes();
    chart1->axisX()->setRange(0, 30);
    myaxis1->setTickCount(8);
    myaxis1->setLabelFormat("%.1f");
    chartview1->chart()->setAxisY(myaxis1, HamiltonianTime);
    chartview1->chart()->setAxisY(myaxis1, KineticTime);
    chartview1->chart()->setAxisY(myaxis1, PotentialTime);


    // chart2
    myaxis2 = new QValueAxis();
    myaxis2->setTickCount(8);
    myaxis2->setMax(1);
    chart2 = createMyChart("Correlation");
    CorrelationTime = createScatterSeries();
    chart2->addSeries(CorrelationTime);
    chartview2 = new QChartView(chart2);
    chart2->createDefaultAxes();
    chart2->axisX()->setRange(0, 30);
    chart2->setAxisY(myaxis2);
    CorrelationTime->attachAxis(myaxis2);


    QLabel * labelx = new QLabel(this);
    labelx->setText("Bx (x10^-6)");
    QLabel * labely = new QLabel(this);
    labely->setText("By (x10^-6)");
    QLabel * labelz = new QLabel(this);
    labelz->setText("Bz (x10^-6)");
    QLabel * labela = new QLabel(this);
    labela->setText("Angle:");
    QLabel * labelom = new QLabel(this);
    labelom->setText("Omega:");
    QLabel * labelmagnet = new QLabel(this);
    labelmagnet->setText("Change Magnet:");
    QLabel * labelMagnetLength = new QLabel(this);
    labelMagnetLength->setText("Magnet Length:");
    QLabel * labelMagnetStrength = new QLabel(this);
    labelMagnetStrength->setText("Magnet Strength:");
    QLabel * labelMagnetLengthUnit = new QLabel(this);
    labelMagnetLengthUnit->setText("(x10^-2) [m]");
    QLabel * labelMagnetStrengthUnit = new QLabel(this);
    labelMagnetStrengthUnit->setText("[C]");


    xSlider          = createSlider();
    ySlider          = createSlider();
    zSlider          = createSlider();
    xselector        = createdoubleSpin(0.1);
    yselector        = createdoubleSpin(0.1);
    zselector        = createdoubleSpin(0.1);
    magnetselector   = createSpin();
    MagnetLength     = createdoubleSpin(0.1);


    MagnetStrength   = createdoubleSpin(0.1);



    labels = new QLabel(this);
    labels->setText("Change Magnet");
    angleselector = createdoubleSpin();
    flipangle     = new QPushButton("Flip", this);
    omegato0      = new QPushButton("0", this);
    MagnetsLoad   = new QPushButton("Load", this);
    MagnetsSave   = new QPushButton("Save", this);
    MagnetsLoad->setToolTip(tr("Load Magnets' angles from a file"));
    MagnetsSave->setToolTip(tr("Save Magnets' angles to a file"));

    //Bfield values
    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXB(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYB(int)));
    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZB(int)));
    connect(xselector, SIGNAL(valueChanged(double)), glWidget, SLOT(setXB(double)));
    connect(yselector, SIGNAL(valueChanged(double)), glWidget, SLOT(setYB(double)));
    connect(zselector, SIGNAL(valueChanged(double)), glWidget, SLOT(setZB(double)));

    connect(magnetselector, SIGNAL(valueChanged(int)), glWidget, SLOT(selectmagnet(int)));
    connect(glWidget, SIGNAL(magnetselected(int)), this, SLOT(setmagnetT(int)));

    connect(flipangle, SIGNAL(clicked()), glWidget, SLOT(flipmagnet()));
    connect(omegato0, SIGNAL(clicked()), glWidget, SLOT(omegato0()));
    connect(angleselector, SIGNAL(valueChanged(double)), glWidget, SLOT(changeangle(double)));
    connect(glWidget, SIGNAL(anglechanged(int)), this, SLOT(setangleT(int)));

    // Magnets' attibutes
    connect(MagnetLength, SIGNAL(valueChanged(double)), glWidget, SLOT(setMagnetLength(double)));
    connect(MagnetStrength, SIGNAL(valueChanged(double)), glWidget, SLOT(setMagnetStrength(double)));

    // Loading and saving
    connect(MagnetsSave, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(MagnetsLoad, SIGNAL(clicked()), this, SLOT(loadFromFile()));

    // Layout:
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->addWidget(glWidget, 0, 0, 2, 1);
    mainLayout->addWidget(labelO, 2, 0, -1, 1);

    mainLayout->addWidget(chartview1, 0, 1, 1, 2);
    mainLayout->addWidget(chartview2, 1, 1, 1, 2);
    mainLayout->setColumnMinimumWidth(1, 250);

    mainLayout->addWidget(xselector, 4, 2);
    mainLayout->addWidget(labelx, 4, 1);
    mainLayout->addWidget(yselector, 3, 2);
    mainLayout->addWidget(labely, 3, 1);
    mainLayout->addWidget(zselector, 2, 2);
    mainLayout->addWidget(labelz, 2, 1);

    mainLayout->addWidget(labels, 1, 5, 1, 3); // magnet attributes

    // System dimensions and Interaction Strength and type:
    mainLayout->addWidget(MagnetsSave, 0, 5);

    mainLayout->addWidget(MagnetsLoad, 0, 6);

    mainLayout->addWidget(labelMagnetLength, 2, 5);

    mainLayout->addWidget(MagnetLength, 2, 6);
    mainLayout->addWidget(labelMagnetLengthUnit, 2, 7);

    mainLayout->addWidget(labelMagnetStrength, 3, 5);

    mainLayout->addWidget(MagnetStrength, 3, 6);
    mainLayout->addWidget(labelMagnetStrengthUnit, 3, 7);

    mainLayout->addWidget(labelmagnet, 4, 5);
    mainLayout->addWidget(magnetselector, 4, 6);
    mainLayout->addWidget(labelom, 5, 5);
    mainLayout->addWidget(omegato0, 5, 6);
    mainLayout->addWidget(labela, 6, 5);
    mainLayout->addWidget(angleselector, 6, 6);
    mainLayout->addWidget(flipangle, 6, 7);


    setLayout(mainLayout);

    xSlider->setValue(0 * 16);
    ySlider->setValue(0 * 16);
    zSlider->setValue(0 * 16);
    setWindowTitle(tr("Magnetic Frustration"));
}

QSlider * Window::createSlider()
{
    QSlider * slider = new QSlider(Qt::Vertical);

    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

QSpinBox * Window::createSpin()
{
    QSpinBox * spin = new QSpinBox();

    spin->setRange(0, 100);
    spin->setSingleStep(1);
    return spin;
}

QDoubleSpinBox * Window::createdoubleSpin(double step)
{
    QDoubleSpinBox * spin = new QDoubleSpinBox();
    spin->setRange(0, 1000000);
    spin->setSingleStep(step);
    return spin;
}

QSplineSeries * Window::createScatterSeries()
{
    QSplineSeries * ScatterSeries1 = new QSplineSeries();
    return ScatterSeries1;
}

QChart * Window::createMyChart(QString s)
{
    QChart * MyChart = new QChart();
    MyChart->setTitle(s);
    return MyChart;
}
