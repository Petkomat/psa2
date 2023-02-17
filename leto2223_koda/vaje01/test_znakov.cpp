#include <iostream>
#include <vector>
#include <string>


int main(){
    std::string x  = "mešiček ž AČŠŽčšž";
    for (auto const& znak : x){
        std::cout << "int(znak): " << (znak + 0) << " ; znak: " << znak << std::endl;
    }
    std::string ch = "č";
    std::cout << (ch[0] == -60) << std::endl;
    std::cout << ((ch[0] + 0) == -60) << std::endl;
}