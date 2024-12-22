#ifndef SIMULATOR_
#define SIMULATOR_

#include <bits/stdc++.h>

#include "Field.hpp"
#include "Fixed.hpp"
#include "VectorField.hpp"
#include "Rnd.hpp"

class Base{
public:
    virtual void build(std::string in, std::string out) = 0;
    virtual void run() = 0;
    virtual void save() = 0;
    virtual ~Base() = default;
};

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
class Simulator : public Base{
public:
    Simulator() {};
    void build(std::string in, std::string out);
    void run() override;
    void save() override;
    ~Simulator() override = default;

private:
    bool is_initialized = false;
    Field field;
    VectorField<vType, Nv, Mv> velocity{};
    VectorField<vfType, Nv, Mv> velocity_flow{};
    int last_use[Nv][Mv]{};
    int UT = 0;
    vType g{};
    pType rho[256]{};
    pType p[Nv][Mv], old_p[Nv][Mv];
    int cur_Tick = 0;

    std::tuple<vfType, bool, std::pair<int, int>> propagate_flow(int x, int y, vfType lim);
    void propagate_stop(int x, int y, bool force = false);
    vType move_prob(int x, int y);
    bool propagate_move(int x, int y, bool is_first);
};

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
void Simulator<pType, vType, vfType, Nv, Mv>::build(std::string in, std::string out){
    is_initialized = true;
    field = std::move(Field(in, out));
    g = field.g;
    for (int i = 0; i < 256; ++i){
        rho[i] = field.rho[i];
    }
}

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
void Simulator<pType, vType, vfType, Nv, Mv>::save(){
    field.save_field();
}


template <typename pType, typename vType, typename vfType, int Nv, int Mv>
std::tuple<vfType, bool, std::pair<int, int>> Simulator<pType, vType, vfType, Nv, Mv>::propagate_flow(int x, int y, vfType lim) {
    last_use[x][y] = UT - 1;
    vfType ret = 0;
    for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field.field[nx][ny] != '#' && last_use[nx][ny] < UT) {
            auto cap = velocity.get(x, y, dx, dy);
            auto flow = velocity_flow.get(x, y, dx, dy);
            if (flow == vfType(cap)) {
                continue;
            }
            // assert(v >= velocity_flow.get(x, y, dx, dy));
            auto vp = min(lim, vfType(cap) - flow);
            if (last_use[nx][ny] == UT - 1) {
                velocity_flow.add(x, y, dx, dy, vp);
                last_use[x][y] = UT;
                // cerr << x << " " << y << " -> " << nx << " " << ny << " " << vp << " / " << lim << "\n";
                return {vp, 1, {nx, ny}};
            }
            auto [t, prop, end] = propagate_flow(nx, ny, vp);
            ret += t;
            if (prop) {
                velocity_flow.add(x, y, dx, dy, t);
                last_use[x][y] = UT;
                // cerr << x << " " << y << " -> " << nx << " " << ny << " " << t << " / " << lim << "\n";
                return {t, prop && end != std::pair(x, y), end};
            }
        }
    }
    last_use[x][y] = UT;
    return {ret, 0, {0, 0}};
}

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
void Simulator<pType, vType, vfType, Nv, Mv>::propagate_stop(int x, int y, bool force) {
    if (!force) {
        bool stop = true;
        for (auto [dx, dy] : deltas) {
            int nx = x + dx, ny = y + dy;
            if (field.field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) > 0) {
                stop = false;
                break;
            }
        }
        if (!stop) {
            return;
        }
    }
    last_use[x][y] = UT;
    for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field.field[nx][ny] == '#' || last_use[nx][ny] == UT || velocity.get(x, y, dx, dy) > 0) {
            continue;
        }
        propagate_stop(nx, ny);
    }
}

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
vType Simulator<pType, vType, vfType, Nv, Mv>::move_prob(int x, int y) {
    vType sum = 0;
    for (size_t i = 0; i < deltas.size(); ++i) {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field.field[nx][ny] == '#' || last_use[nx][ny] == UT) {
            continue;
        }
        auto v = velocity.get(x, y, dx, dy);
        if (v < 0) {
            continue;
        }
        sum += v;
    }
    return sum;
}

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
bool Simulator<pType, vType, vfType, Nv, Mv>::propagate_move(int x, int y, bool is_first) {
    last_use[x][y] = UT - is_first;
    bool ret = false;
    int nx = -1, ny = -1;
    do {
        std::array<vType, deltas.size()> tres;
        vType sum = 0;
        for (size_t i = 0; i < deltas.size(); ++i) {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field.field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                tres[i] = sum;
                continue;
            }
            auto v = velocity.get(x, y, dx, dy);
            if (v < 0) {
                tres[i] = sum;
                continue;
            }
            sum += v;
            tres[i] = sum;
        }

        if (sum == 0) {
            break;
        }

        vType p = Rnd::random01<vType>() * sum;
        size_t d = std::ranges::upper_bound(tres, p) - tres.begin();

        auto [dx, dy] = deltas[d];
        nx = x + dx;
        ny = y + dy;
        assert(velocity.get(x, y, dx, dy) > 0 && field.field[nx][ny] != '#' && last_use[nx][ny] < UT);

        ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
    } while (!ret);
    last_use[x][y] = UT;
    for (size_t i = 0; i < deltas.size(); ++i) {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field.field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) < 0) {
            propagate_stop(nx, ny);
        }
    }
    if (ret) {
        if (!is_first) {
            std::swap(field.field[x][y], field.field[nx][ny]);
            std::swap(p[x][y], p[nx][ny]);
            std::swap(velocity.v[x][y], velocity.v[nx][ny]);
        }
    }
    return ret;
}

template <typename pType, typename vType, typename vfType, int Nv, int Mv>
void Simulator<pType, vType, vfType, Nv, Mv>::run(){
    if (!is_initialized){
        std::cout << "you cannot run uninitialized simulator\n";
        exit(-1);
    }
    auto& myfield = field.field;
    constexpr size_t T = 1'000'000;
    int dirs[Nv][Mv]{};

    for (size_t x = 0; x < Nv; ++x) {
        for (size_t y = 0; y < Mv; ++y) {
            if (myfield[x][y] == '#')
                continue;
            for (auto [dx, dy] : deltas) {
                dirs[x][y] += (myfield[x + dx][y + dy] != '#');
            }
        }
    }

    while (cur_Tick++ < T) {
        
        // Apply external forces
        for (size_t x = 0; x < Nv; ++x) {
            for (size_t y = 0; y < Mv; ++y) {
                if (myfield[x][y] == '#')
                    continue;
                if (myfield[x + 1][y] != '#')
                    velocity.add(x, y, 1, 0, g);
            }
        }

        // Apply forces from p
        memcpy(old_p, p, sizeof(p));
        for (size_t x = 0; x < Nv; ++x) {
            for (size_t y = 0; y < Mv; ++y) {
                if (myfield[x][y] == '#')
                    continue;
                for (auto [dx, dy] : deltas) {
                    int nx = x + dx, ny = y + dy;
                    if (myfield[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y]) {
                        auto delta_p = old_p[x][y] - old_p[nx][ny];
                        auto force = delta_p;
                        auto &contr = velocity.get(nx, ny, -dx, -dy);
                        if (pType(contr) * rho[myfield[nx][ny]] >= force) {
                            contr -= vType(force / rho[myfield[nx][ny]]);
                            continue;
                        }
                        force -= pType(contr) * rho[myfield[nx][ny]];
                        contr = 0;
                        velocity.add(x, y, dx, dy, vType(force / rho[myfield[x][y]]));
                        p[x][y] -= force / pType(dirs[x][y]);
                    }
                }
            }
        }

        // Make flow from velocities
        velocity_flow = {};
        bool prop = false;
        do {
            UT += 2;
            prop = 0;
            for (size_t x = 0; x < Nv; ++x) {
                for (size_t y = 0; y < Mv; ++y) {
                    if (myfield[x][y] != '#' && last_use[x][y] != UT) {
                        auto [t, local_prop, _] = propagate_flow(x, y, 1);
                        if (t > 0) {
                            prop = 1;
                        }
                    }
                }
            }
        } while (prop);

        // Recalculate pdeltas with kinetic energy
        for (size_t x = 0; x < Nv; ++x) {
            for (size_t y = 0; y < Mv; ++y) {
                if (myfield[x][y] == '#')
                    continue;
                for (auto [dx, dy] : deltas) {
                    auto old_v = velocity.get(x, y, dx, dy);
                    auto new_v = velocity_flow.get(x, y, dx, dy);
                    if (old_v > 0) {
                        assert(new_v <= old_v);
                        velocity.get(x, y, dx, dy) = vType(new_v);
                        pType force = (pType(old_v) - pType(new_v)) * rho[myfield[x][y]];
                        if (myfield[x][y] == '.')
                            force *= pType(0.8);
                        if (myfield[x + dx][y + dy] == '#') {
                            p[x][y] += force / pType(dirs[x][y]);
                        } else {
                            p[x + dx][y + dy] += force / pType(dirs[x + dx][y + dy]);
                        }
                    }
                }
            }
        }

        UT += 2;
        prop = false;
        for (size_t x = 0; x < Nv; ++x) {
            for (size_t y = 0; y < Mv; ++y) {
                if (myfield[x][y] != '#' && last_use[x][y] != UT) {
                    if (Rnd::random01<vType>() < move_prob(x, y)) {
                        prop = true;
                        propagate_move(x, y, true);
                    } else {
                        propagate_stop(x, y, true);
                    }
                }
            }
        }

        if (prop) {
            std::cout << "Tick " << cur_Tick << ":\n";
            field.print_field();
        }
    }
}
#endif
