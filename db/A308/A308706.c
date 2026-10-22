/*
 * A308706  Number of chiral pairs of set partitions of a primitive cycle of n
 *          elements having exactly two different elements.
 * https://oeis.org/A308706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = ((1/(2*n)) * Sum_{odd d|n} mu(d) * 2^(n/d) * -
 *            Sum_{d|n} mu(n/d) * 2^floor(d/2)) / 2 * = (Sum_{odd d|n} mu(d) *
 *            2^(n/d) * - 2*n * Sum_{d|n} mu(n/d) * 2^floor(d/2)) / (4*n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   compare_seq_t, mu_of, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mu_of.h"
#include "../utils/mul_small.h"
#include "../utils/compare_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Positive and negative accumulators for S1 and S2 */
    seq_t pos1 = 0, neg1 = 0;   /* S1: Sum_{odd d|n} mu(d) * 2^(n/d)  */
    seq_t pos2 = 0, neg2 = 0;   /* S2: Sum_{d|n} mu(n/d) * 2^floor(d/2) */

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;
        idx_t d2 = n / d;          /* counterpart divisor */

        /* ----- divisor d ----- */

        /* S1: odd d */
        if (d & 1U) {
            int mu = mu_of(d);
            seq_t term = pow2(n / d);   /* 2^(n/d) */
            if (mu > 0)
                pos1 += term;
            else if (mu < 0)
                neg1 += term;
        }

        /* S2: mu(n/d) * 2^floor(d/2) */
        int mu_nd = mu_of(d2);           /* mu(n/d) */
        seq_t term2 = pow2(d >> 1);      /* 2^floor(d/2) */
        if (mu_nd > 0)
            pos2 += term2;
        else if (mu_nd < 0)
            neg2 += term2;

        if (d2 == d)
            continue;   /* already processed this divisor */

        /* ----- divisor d2 ----- */

        /* S1: odd d2 */
        if (d2 & 1U) {
            int mu = mu_of(d2);
            seq_t term = pow2(d);        /* 2^(n/d2) = 2^d */
            if (mu > 0)
                pos1 += term;
            else if (mu < 0)
                neg1 += term;
        }

        /* S2: mu(d) * 2^floor(d2/2) */
        int mu_d = mu_of(d);             /* mu(d) */
        seq_t term2b = pow2(d2 >> 1);    /* 2^floor(d2/2) */
        if (mu_d > 0)
            pos2 += term2b;
        else if (mu_d < 0)
            neg2 += term2b;
    }

    /* Compute numerator N = (pos1 + 2*n*neg2) - (neg1 + 2*n*pos2) */
    unsigned int two_n = (unsigned int)(2 * n);       /* 2*n fits in unsigned int for n <= 10000 */
    seq_t term_p = pos1 + mul_small(neg2, two_n);     /* pos1 + 2*n*neg2 */
    seq_t term_m = neg1 + mul_small(pos2, two_n);     /* neg1 + 2*n*pos2 */

    seq_t N;
    if (compare_seq_t(&term_p, &term_m) < 0)
        N = 0;
    else
        N = term_p - term_m;

    /* Denominator = 4*n */
    seq_t denom = mul_small((seq_t)n, 4);

    return N / denom;
}
