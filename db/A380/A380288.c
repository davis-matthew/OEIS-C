/*
 * A380288  a(n) is the number of divisors d of n such that 2^d - 1 is not
 *          prime.
 * https://oeis.org/A380288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) – #{d | n : 2^d – 1 is a Mersenne prime}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Known Mersenne‑prime exponents up to 10 000. */
static const uint64_t mersenne_exponents[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL,
    17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL,
    1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL
};
static const size_t mersenne_count =
    sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, should not happen */

    uint64_t N = (uint64_t)n;
    seq_t divisor_count = 1;

    uint64_t tmp = N;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            divisor_count *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) {                  /* remaining prime factor */
        divisor_count *= 2;
    }

    /* Count how many Mersenne‑prime exponents divide N. */
    uint64_t mer_cnt = 0;
    for (size_t i = 0; i < mersenne_count; ++i) {
        uint64_t e = mersenne_exponents[i];
        if (e > N) break;
        if (N % e == 0) ++mer_cnt;
    }

    return divisor_count - (seq_t)mer_cnt;
}
