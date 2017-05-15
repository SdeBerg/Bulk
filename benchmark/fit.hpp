#include <cassert>
#include <vector>

#include <experimental/optional>

namespace std {
template <typename T>
using optional = experimental::optional<T>;
}

namespace bulk {

template <typename FwdIterator>
auto average(FwdIterator first, FwdIterator last) {
    auto k = 0u;
    auto a = *first;
    a = 0;
    for (; first != last; ++first) {
        a += *first;
        ++k;
    }
    return a / k;
}

/** Compute the 'zip' of two vectors. */
template <typename T, typename U>
std::vector<std::pair<T, U>> zip(std::vector<T> xs, std::vector<U> ys) {
    std::vector<std::pair<T, U>> result;
    for (size_t i = 0; i < (xs.size() < ys.size() ? xs.size() : ys.size());
         ++i) {
        result.push_back(std::make_pair(xs[i], ys[i]));
    }
    return result;
}

std::optional<std::pair<double, double>> fit(const std::vector<size_t>& xs,
                                             const std::vector<double>& ys) {
    if (xs.size() < 2 || ys.size() != xs.size()) {
        return std::optional<std::pair<double, double>>();
    }

    auto sum = [](const auto& zs) {
        return std::accumulate(zs.begin(), zs.end(), 0);
    };

    auto avg_x = sum(xs) / (double)xs.size();
    auto avg_y = sum(ys) / (double)ys.size();
    auto points = zip(xs, ys);

    auto num = std::accumulate(
        points.begin(), points.end(), 0.0, [=](double a, auto p) {
            return a + (p.first - avg_x) * (p.second - avg_y);
        });

    auto denum =
        std::accumulate(xs.begin(), xs.end(), 0.0, [=](double a, double x) {
            return a + (x - avg_x) * (x - avg_x);
        });

    auto g = num / denum;
    auto l = avg_y - g * avg_x;

    return std::optional<std::pair<double, double>>({l, g});
}

}