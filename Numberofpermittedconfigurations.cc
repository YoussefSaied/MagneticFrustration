#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

int main()
{
    vector<vector<size_t> > v;
    for (size_t a = 0; a <= 12; a++) {
        size_t limb = (12 - a);
        for (size_t b = 0; b <= limb; b++) {
            size_t limc = min((12 - a - b), size_t(20));
            for (size_t c = 0; c <= limc; c++) {
                size_t limd = min((12 - a - b - c), (20 - c));
                for (size_t d = 0; d <= limd; d++) {
                    size_t lime = min((12 - a - b - c - d), (20 - c - d));
                    for (size_t e = 0; e <= lime; e++) {
                        size_t limf = min((12 - a - b - c - d - e), (20 - c - d - e));
                        for (size_t f = 0; f <= limf; f++) {
                            size_t limg = min((12 - a - b - c - d - e - f), size_t(10 - 0.5 * (c + d + e + f)));
                            for (size_t g = 0; g <= limg; g++) {
                                size_t limh = min((12 - a - b - c - d - e - f - g),
                                    size_t(10 - g - 0.5 * (c + d + e + f)));
                                for (size_t h = 0; h <= limh; h++) {
                                    if ((a + b + c + d + e + f + g + h) == 12 and ((c + d + e + f) * 2 + (g + h) * 4) ==
                                      40)
                                    {
                                        v.push_back({ a, b, c, d, e, f, g, h });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vector<vector<size_t> > w;
    for (size_t a = 0; a <= 12; a++) {
        size_t limb = (12 - a);
        for (size_t b = 0; b <= limb; b++) {
            size_t limc = min((12 - a - b), size_t(20));
            for (size_t c = 0; c <= limc; c++) {
                size_t limd = min((12 - a - b - c), (20 - c));
                for (size_t d = 0; d <= limd; d++) {
                    size_t lime = min((12 - a - b - c - d), (20 - c - d));
                    for (size_t e = 0; e <= lime; e++) {
                        size_t limf = min((12 - a - b - c - d - e), (20 - c - d - e));
                        for (size_t f = 0; f <= limf; f++) {
                            size_t limg = min((12 - a - b - c - d - e - f), size_t(10 - 0.5 * (c + d + e + f)));
                            for (size_t g = 0; g <= limg; g++) {
                                size_t limh = min((12 - a - b - c - d - e - f - g),
                                    size_t(10 - g - 0.5 * (c + d + e + f)));
                                for (size_t h = 0; h <= limh; h++) {
                                    if ((a + b + c + d + e + f + g + h) == 12 and ((c + d + e + f) * 2 + (g + h) * 4) ==
                                      40)
                                    {
                                        bool exists = 0;
                                        for (auto occ: w) {
                                            if (occ[0] ==
                                              b and occ[1] == a and occ[2] == d and occ[3] == c and occ[4] ==
                                              f and occ[5] ==
                                              e and occ[6] == h and occ[7] == g)
                                            {
                                                exists = 1;
                                            }
                                        }
                                        if (!exists) {
                                            w.push_back({ a, b, c, d, e, f, g, h });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vector<vector<size_t> > y;
    for (size_t a = 0; a <= 10; ++a) {
        for (size_t b = 0; b <= 10; b++) {
            for (size_t c = 0; c <= 12; c++) {
                size_t num2FF   = (2 * a + b);
                size_t num2WW   = ( 2 * a + 3 * b + 4 * c);
                size_t num2FW   = ( a + 1 * b + c);
                size_t numFaces = (a + b + c);
                size_t numF     = num2FF + num2FW / 2;
                size_t numW     = num2WW + num2FW / 2;
                if (num2WW <= 40 and num2FF <= 20 and numFaces == 12 and num2FW <=
                  40 and numF == 20 and numW == 40)
                {
                    y.push_back({ a, b, c });
                }
            }
        }
    }

    vector<vector<size_t> > z;

    for (size_t a = 0; a <= 12; ++a) {
        size_t maxb = (12 - a) / 2;
        for (size_t b = 0; b <= maxb; ++b) {
            size_t maxc = (12 - a - 2 * b) / 3;
            for (size_t c = 0; c <= maxc; ++c) {
                size_t maxd = min((20 - a - b - c), (34 - 2 * a - b) / 2);
                for (size_t d = 0; d <= maxd; ++d) {
                    size_t maxe = 12 - a - 2 * b - 3 * c;
                    for (size_t e = 0; e <= maxe; e++) {
                        size_t maxf = (12 - a - 2 * b - 3 * c - e) / 2;
                        for (size_t f = 0; f <= maxf; f++) {
                            if (d + e + f == 14 and a + b + c + d + e + f == 20 and a + 2 * b + 3 * c + e + 2 * f ==
                              12 and 2 * a + b + 2 * d + e == 34)
                            {
                                z.push_back({ a, b, c, d, e, f });
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "without sym:" << v.size() << endl;
    cout << "with sym:" << w.size() << endl;
    cout << "Most Basic:" << y.size() << endl;
    for (auto elem: y) {
        std::cout << elem[0] << " , " << elem[1] << " , " << elem[2] << '\n';
    }
    cout << "Vertexconfiguration: " << z.size() << endl;

    for (auto elem: z) {
        std::cout << elem[0] << " , " << elem[1] << " , " << elem[2] << " , " << elem[3] << " , " << elem[4] << " , "
                  << elem[5] << '\n';
    }
} // main
