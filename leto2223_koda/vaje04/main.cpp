#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "kdd.h"

using std::vector;


int main()
{
    vector<vector<double>> xs;
    int n = 10;
    int d = 2;
    std::default_random_engine re;
    std::uniform_real_distribution unif(0.0, 1.0);
    for (int i = 0; i < n; i++){
        vector<double> x;
        for (int j = 0; j < d; j++){
            x.push_back(unif(re));
        }
        xs.push_back(x);
    }
    std::cout << "is this a real life " << xs.size() <<  std::endl;

    auto drevo = KDDrevo::narediDrevo(xs, 0);
    for (auto const & x : xs){
        auto [optX, optR] = drevo -> najdi(x);
        std::cout << optX[0] << " na razd. " << optR << std::endl;
    }
    return 0;
}
