/*
 * A283273  Number of ways to write n as x^2 + y^2 + z^2 with x,y integers and
 *          z a nonnegative integer such that x + 2*y + 3*z is a square or
 *          twice a square.
 * https://oeis.org/A283273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A283273 – Number of ordered triples (x,y,z) with
 *
 *   x^2 + y^2 + z^2 = n
 *   z ≥ 0 , x,y ∈ ℤ
 *   s = x + 2*y + 3*z is a perfect square or twice a perfect square.
 *
 *   This implementation enumerates every admissible (z,x,y),
 *   checks the square condition via floor_sqrt(seq_t) and counts
 *   qualifying triples.  It runs in O(n^{3/2}) time and uses only
 *   the arithmetic operators allowed by the specification.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) available */

static inline idx_t isqrt_idx(idx_t v)       /* integer sqrt for idx_t */
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;                  /* highest power of four ≤ 2^64-1 */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t zmax = isqrt_idx(n);                /* maximum z */

    for (idx_t z = 0; z <= zmax; ++z) {
        seq_t zsq   = (seq_t)z * (seq_t)z;
        seq_t rem_n = (seq_t)n - zsq;
        if (rem_n < 0) break;                /* should not occur */

        idx_t xmax = (idx_t)floor_sqrt(rem_n);   /* max |x| */

        for (idx_t xu = 0; xu <= xmax; ++xu) {
            seq_t xsq    = (seq_t)xu * (seq_t)xu;
            seq_t rem_xy = rem_n - xsq;
            if (rem_xy < 0) continue;

            idx_t yu = (idx_t)floor_sqrt(rem_xy);   /* max |y| */
            if ((seq_t)yu * yu != rem_xy) continue;/* rem_xy not a square */

            for (int xsig = 0; xsig < 2; ++xsig) {   /* signs for x */
                long long xs = (long long)xu;
                if (xsig == 1) xs = -xs;
                if (xu == 0 && xsig == 1) continue;   /* skip duplicate */

                for (int ysig = 0; ysig < 2; ++ysig) { /* signs for y */
                    long long ys = (long long)yu;
                    if (ysig == 1) ys = -ys;
                    if (yu == 0 && ysig == 1) continue; /* skip duplicate */

                    long long s = xs + 2 * ys + 3 * (long long)z;
                    if (s < 0) continue;               /* s must be non‑negative */

                    seq_t ss = (seq_t)s;

                    /* check if ss is a perfect square */
                    idx_t t = (idx_t)floor_sqrt(ss);
                    if ((seq_t)t * t == ss) {
                        ++total;
                        continue;
                    }

                    /* check if ss is twice a perfect square */
                    if ((ss & 1) == 0) {                 /* ss even */
                        seq_t half = ss >> 1;
                        idx_t u = (idx_t)floor_sqrt(half);
                        if ((seq_t)u * u == half) {
                            ++total;
                        }
                    }
                }
            }
        }
    }
    return total;
}
