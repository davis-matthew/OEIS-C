/*
 * A329443  a(n) is the GCD of the binary representation of n interpreted in
 *          any numeric base.
 * https://oeis.org/A329443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = GCD of the binary representation of n interpreted in any
 *            numeric base > 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include "../utils/_gcd64.h"

/* Helper: Euclidean algorithm for 64‑bit integers */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Highest set bit position, i.e. degree of the polynomial */
    int deg = 0;
    idx_t tmp = n;
    while ((tmp >>= 1) != 0) ++deg;

    uint64_t g = 0;

    /* Base 0 contributes only the least‑significant bit */
    if (n & 1ULL) g = 1;

    /* Evaluate the polynomial for bases 1 … deg */
    for (int base = 1; base <= deg; ++base) {
        uint64_t val = 0;
        for (int i = deg; i >= 0; --i) {
            if ((n >> i) & 1ULL)
                val = val * (uint64_t)base + 1;
            else
                val = val * (uint64_t)base;
        }

        if (g == 0)
            g = val;
        else
            g = _gcd64(g, val);

        if (g == 1) break;   /* cannot become smaller */
    }

    return (seq_t)g;
}
