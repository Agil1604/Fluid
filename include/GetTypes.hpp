#ifndef GETTYPES_
#define GETTYPES_

#ifndef SIZES
#define SIZES
#endif

#include "Fixed.hpp"
#include "Simulator.hpp"
#include <string>

#define FLOAT 0
#define DOUBLE 1
#define FIXED(n, k) 100*n + k 
#define FAST_FIXED(n, k) 10000*n + k
#define S(a, b) std::pair<int, int>(a, b)

#define DEFAULT -1
#define COMBINATIONS t.size()*t.size()*t.size()*s.size()

class ParseTypes{
public:
    ParseTypes(int argc, char* argv[]);
    int p  = DEFAULT;
    int v  = DEFAULT;
    int vf = DEFAULT;
    std::string inputfile = "../input.txt";
    std::string outputfile = "../output.txt";
private:
    void parseFlag(std::string& flag);
};

constexpr std::array t{TYPES};
constexpr std::array s{std::pair{0, 0}, SIZES};

template <int num>
auto get(){
    if constexpr (num == FLOAT) {
        return std::type_identity<float>{};
    } else if constexpr (num == DOUBLE) {
        return std::type_identity<double>{};
    } else if constexpr (num < 10000) {
        return std::type_identity<Fixed<num/100,num%100>>{};
    } else {
        return std::type_identity<FastFixed<num/10000,num%10000>>{};
    }
}

template <int num>
using numType = typename decltype(get<num>())::type;

template <typename P, typename V, typename VF, size_t N, size_t M>
std::unique_ptr<Base> generate() {
    return std::make_unique<Simulator<P, V, VF, N, M>>();
}

template <int index>
constexpr auto CreateSimulators(){
    auto res = CreateSimulators<index+1>();
    constexpr int type1 = index/(t.size()*t.size()*s.size());
    constexpr int type2 = index%(t.size()*t.size()*s.size())/(t.size()*s.size());
    constexpr int type3 = index%(t.size()*s.size())/s.size();
    res[index] = generate<numType<t[type1]>, numType<t[type2]>, numType<t[type3]>, s[index%s.size()].first, s[index%s.size()].second>;
    return res;
}

template <>
consteval auto CreateSimulators<COMBINATIONS>() {
    return std::array<std::unique_ptr<Base>(*)(), COMBINATIONS>();
}

template <int index>
constexpr auto SaveSimulatorIndex(){
    auto res = SaveSimulatorIndex<index+1>();
    constexpr int type1 = index/(t.size()*t.size()*s.size());
    constexpr int type2 = index%(t.size()*t.size()*s.size())/(t.size()*s.size());
    constexpr int type3 = index%(t.size()*s.size())/s.size();
    res[index] = {t[type1], t[type2], t[type3], s[index%s.size()].first, s[index%s.size()].second};
    return res;
}

template <>
consteval auto SaveSimulatorIndex<COMBINATIONS>() {
    return std::array<std::tuple<int, int, int, size_t, size_t>, COMBINATIONS>();
}


constexpr auto tmp1 = SaveSimulatorIndex<0>;
constexpr auto tmp2 = CreateSimulators<0>;
constexpr auto types = tmp1();
constexpr auto simulators = tmp2();
#endif