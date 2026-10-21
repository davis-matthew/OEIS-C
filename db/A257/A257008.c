/*
 * A257008  Number of Zagier-reduced binary quadratic forms of discriminant
 *          n^2 + 4.
 * https://oeis.org/A257008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Zagier‑reduced binary quadratic forms of
 *            discriminant n^2+4 This uses the definition: count pairs (h,k)
 *            with |k| < √D , k^2 ≡ D (mod 4), 2h + k > 0 and (2h + k)^2 > D,
 *            and h | (D − k^2)/4
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"   /* provides floor_sqrt for seq_t */

/* external linkage */
seq_t a(idx_t n)
{
    /* Discriminant D = n^2 + 4 */
    seq_t D = (seq_t)n * (seq_t)n + (seq_t)4ULL;

    /* compute integer floor of √D */
    seq_t sqrtD_seq = floor_sqrt(D);
    unsigned long long sqrtD = (unsigned long long)sqrtD_seq;

    seq_t count = 0;          /* result accumulator */

    /* iterate over all integer k with |k| < √D */
    for (int64_t k = -(int64_t)sqrtD; k <= (int64_t)sqrtD; ++k)
    {
        /* k^2 must be < D (automatically true for our range) */
        seq_t k2 = (seq_t)k * (seq_t)k;
        if (k2 >= D) continue;

        /* k^2 ≡ D (mod 4) */
        if ((k2 & (seq_t)3) != (D & (seq_t)3)) continue;

        /* t = (D - k^2) / 4, guaranteed integer */
        seq_t t = (D - k2) >> 2;
        if (t == 0) continue;      /* no positive divisors if t==0 */

        /* factor t by trial division over divisors d up to √t */
        seq_t sqrt_t_seq = floor_sqrt(t);
        unsigned long long sqrt_t = (unsigned long long)sqrt_t_seq;

        for (unsigned long long d = 1; d <= sqrt_t; ++d)
        {
            if ((t % (seq_t)d) != 0) continue;

            /* first divisor h1 = d */
            seq_t h1 = (seq_t)d;
            int64_t lhs1 = 2LL * (int64_t)h1 + k;            /* 2h + k */
            if (lhs1 > 0) {
                seq_t lhs1_sq = (seq_t)lhs1 * (seq_t)lhs1;
                if (lhs1_sq > D) ++count;
            }

            /* second divisor h2 = t / d, if distinct */
            if (d * d != (unsigned long long)t) {
                seq_t h2 = t / (seq_t)d;
                int64_t lhs2 = 2LL * (int64_t)h2 + k;           /* 2h + k */
                if (lhs2 > 0) {
                    seq_t lhs2_sq = (seq_t)lhs2 * (seq_t)lhs2;
                    if (lhs2_sq > D) ++count;
                }
            }
        }
    }

    return count;
}
