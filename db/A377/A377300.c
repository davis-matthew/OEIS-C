/*
 * A377300  G.f.: Sum_{k>=1} k * x^(k*(7*k - 7 + 2)/2) / (1 - x^k).
 * https://oeis.org/A377300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ k for all k ≥ 1 such that * n ≥ k*(7*k-5)/2 and (n -
 *            k*(7*k-5)/2) mod k = 0 * (otherwise k contributes 0). The sum is
 *            over k, adding k once * for each suitable k
 * Range:     u8 n <= 1252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    seq_t sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        seq_t tk = (seq_t)k;
        seq_t ak = tk * ((seq_t)7 * tk - 5) / 2;   /* exponent for this k */

        if (ak > (seq_t)n) break;                 /* no larger k can contribute */

        /* check divisibility condition */
        idx_t rem = (n - (idx_t)ak) % k;
        if (rem == 0)
            sum += tk;
    }
    return sum;
}
