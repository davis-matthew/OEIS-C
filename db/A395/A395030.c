/*
 * A395030  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x,y,z are positive integers satisfying x >= y >= z.
 * https://oeis.org/A395030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x, y, z) of positive integers *
 *            satisfying x^2 + y*z = n with x >= y >= z
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t isqrt_u64(idx_t v) {
    /* integer square root: floor(sqrt(v)) */
    if (v < 2) return v;
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > v / mid) {
            hi = mid - 1;
        } else {
            lo = mid;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n == 0) return 0;       /* no positive triples for n = 0 */

    /* Loop over z from 1 to floor(sqrt(n)) */
    idx_t maxZ = isqrt_u64(n);
    for (idx_t z = 1; z <= maxZ; ++z) {
        /* For fixed z, y must satisfy y >= z and y*z <= n-1
         * so y <= floor((n-1)/z).  If n <= 1 this loop is skipped. */
        if (n <= 1) continue;
        idx_t maxY = (n - 1) / z;
        for (idx_t y = z; y <= maxY; ++y) {
            idx_t prod = y * z;        /* fits in idx_t because prod <= n-1 */
            idx_t residue = n - prod;  /* x^2 must equal this */
            idx_t x = isqrt_u64(residue);
            if (x * x != residue) continue; /* residue not a perfect square */
            if (x < y) continue;             /* enforce x >= y */
            /* y >= z already by loop and z <= y by definition */
            ++count;
        }
    }
    return count;
}
