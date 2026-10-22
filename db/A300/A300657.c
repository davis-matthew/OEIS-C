/*
 * A300657  a(n) = Sum_{d|n} sigma(d) mod d.
 * https://oeis.org/A300657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (sigma(d) mod d)
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A300657)                                        */

#include <stdint.h>
#include "../utils/sigma_of.h"

/* Integer square‑root for a 64‑bit unsigned value.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* OEIS sequence A300657 */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0; return 0 for safety */
        return 0;

    seq_t total = 0;
    idx_t root  = isqrt_u64(n);

    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t sigma1 = sigma_of(d1);
            total += sigma1 % (seq_t)d1;

            if (d1 != d2) {
                seq_t sigma2 = sigma_of(d2);
                total += sigma2 % (seq_t)d2;
            }
        }
    }
    return total;
}
