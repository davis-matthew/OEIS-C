/*
 * A386260  Maximum exponent in the prime factorization of the exponent of the
 *          highest power of 2 dividing 4*n.
 * https://oeis.org/A386260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A051903(v2(4*n)) = A051903(2 + v2(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute v2(n) */
    unsigned v2 = 0;
    uint64_t r = n;
    while ((r & 1ULL) == 0) {
        ++v2;
        r >>= 1;
    }

    /* exponent of 2 in 4*n */
    unsigned e = v2 + 2;               /* e <= 66 for 64‑bit n */

    /* factor e and find maximum exponent in its prime factorization */
    unsigned max_exp = 0;
    unsigned x = e;
    for (unsigned p = 2; p * p <= x; ++p) {
        unsigned cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (cnt > max_exp) max_exp = cnt;
    }
    if (x > 1) {          /* remaining prime factor with exponent 1 */
        if (1 > max_exp) max_exp = 1;
    }

    return (seq_t)max_exp;
}
