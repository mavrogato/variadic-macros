
#include <iostream>
#include <iomanip>
#include <string_view>
#include <array>
#include <algorithm>


#define SYMLIST(...) symlist(#__VA_ARGS__)

template <size_t N>
struct symlist : std::array<char, N> {
    using basis = std::array<char, N>;
    constexpr symlist(char const (&src)[N]) noexcept {
        std::copy_n(src, N, basis::begin());
    }
    [[deprecated]]
    constexpr auto size() const noexcept {
        return 1 + std::count_if(basis::begin(), basis::end(), [](auto item) {
            return item == ',';
        });
    }
    struct iterator {
        basis const& src;
        mutable typename basis::const_iterator cur;
        constexpr iterator(basis const& src, typename basis::const_iterator cur) noexcept
            : src(src)
            , cur(cur)
            {
            }
        constexpr auto operator*() const noexcept {
            std::string_view tmp(cur, src.end());
            tmp = tmp.substr(0, tmp.find_first_of(','));
            while (tmp.front() == ' ') {
                tmp = tmp.substr(1);
            }
            while (tmp.back() == ' ' || tmp.back() == '\0') {
                tmp = tmp.substr(0, tmp.size() - 1);
            }
            return tmp;
        }
        constexpr auto& operator++() const noexcept {
            std::string_view tmp(cur, src.end());
            auto next = tmp.find_first_of(',');
            if (next != std::string_view::npos) {
                cur += (next + 1);
            }
            else {
                cur = src.end();
            }
            return *this;
        }
        constexpr friend bool operator!=(iterator lhs, iterator rhs) noexcept {
            return lhs.cur != rhs.cur;
        }
    };
    constexpr auto begin() const noexcept {
        return iterator(static_cast<basis const&>(*this), basis::begin());
    }
    constexpr auto end() const noexcept {
        return iterator(static_cast<basis const&>(*this), basis::end());
    }
};


int main() {
    constexpr auto test = SYMLIST( boo, foo, woo, bar, baz );
    std::cout << sizeof (test) << std::endl;
    for (auto item : test) {
        std::cout << '"' << item << '"' << std::endl;
    }
    return 0;
}
