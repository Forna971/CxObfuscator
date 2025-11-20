#pragma once
#include <string>
#include <array>
#include <utility>

namespace cx {

    template <size_t S>
    constexpr unsigned int generate_seed(const char(&str)[S]) {
        unsigned int key = S;
        for (size_t i = 0; i < S; ++i) {
            key = (key ^ str[i]) * 16777619;
        }
        return key;
    }

    template <typename CharT, size_t N, unsigned int Key>
    class container {
    public:
        constexpr container(const CharT* str) {
            for (size_t i = 0; i < N; ++i) {
                buffer[i] = str[i] ^ (Key >> ((i % 4) * 8));
            }
        }

        const CharT* decrypt() {
            if (decrypted) {
                return buffer.data();
            }
            for (size_t i = 0; i < N; ++i) {
                buffer[i] ^= (Key >> ((i % 4) * 8));
            }
            decrypted = true;
            return buffer.data();
        }

    private:
        std::array<CharT, N> buffer{};
        bool decrypted = false;
    };
}

#define sk(str)                                                             \
    ([] {                                                                   \
        constexpr auto key = cx::generate_seed(__TIME__);                   \
        constexpr auto data = cx::container<char, sizeof(str), key>(str);   \
        return data;                                                        \
    }())                                                                    \
    .decrypt()

#define sk_w(str)                                                                    \
    ([] {                                                                            \
        constexpr auto key = cx::generate_seed(__TIME__);                            \
        constexpr auto data = cx::container<wchar_t, sizeof(str) / 2, key>(str);     \
        return data;                                                                 \
    }())                                                                             \
    .decrypt()
