/*
 * A338822  a(n) is the number of divisors of n whose last digits equal the
 *          number of digits of n.
 * https://oeis.org/A338822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ d | d | n and d mod 10^k = D }| where D = number of
 *            decimal digits of n k = number of decimal digits of D 10^k is
 *            the modulus
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_count.h"    /* uint64_t digit_count(uint64_t n); */
#include "../utils/floor_isqrt_u64.h"/* idx_t floor_isqrt_u64(idx_t n); */

seq_t a(idx_t n)
{
    /* number of digits of n */
    uint64_t D = digit_count(n);

    /* number of digits of D (k) */
    uint64_t tmp = D;
    unsigned k = 0;
    while (tmp > 0) {
        ++k;
        tmp /= 10;
    }

    /* modulus 10^k */
    idx_t mod = 1;
    for (unsigned i = 0; i < k; ++i)
        mod *= 10;

    seq_t count = 0;
    idx_t root = floor_isqrt_u64(n);
    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i % mod == D) ++count;
            if (j != i && j % mod == D) ++count;
        }
    }
    return count;
}
