/*
 * A332881  If n = Product (p_j^k_j) then a(n) = denominator of Product (1 +
 *          1/p_j).
 * https://oeis.org/A332881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} mu(d)^2/d (i.e. the denominator
 *            of the sum of reciprocals of the squarefree divisors of n)
 * Range:     u8 n <= 256, u16 n <= 9875, u32 n <= 9875, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, though n >= 1 in the OEIS indexing */

    uint64_t nn = (uint64_t)n;

    /* factor_u64 needs preallocated arrays */
    uint64_t pr[64];
    uint64_t ex[64];          /* not used, only the primes are required */
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    /* product of distinct primes (rad(n)) in 128‑bit to avoid overflow */
    unsigned __int128 rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= pr[i];

    /* cancel prime factors that also divide any (q+1) */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t q = pr[j];
            if (((unsigned __int128)q + 1) % p == 0) {
                rad /= p;
                break;
            }
        }
    }

    return (seq_t)rad;
}
