/*
 * A296191  Numerators of the Harary index of the n X n grid graph.
 * https://oeis.org/A296191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the Harary index of the n×n grid graph. H(n)
 *            = Σ_{d=1}^{2(n-1)} N_d / d, where N_d is the number of unordered
 *            vertex pairs at Manhattan distance d. a(n) is the numerator of
 *            H(n) in lowest terms
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd, sum1_idx, sum2_idx, sum_nd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sum1_idx.h"
#include "../utils/sum2_idx.h"
#include "../utils/sum_nd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t num = 0, den = 1;
    idx_t maxd = 2 * (n - 1);

    for (idx_t d = 1; d <= maxd; ++d)
    {
        seq_t N_d;
        if (d <= n - 1)
        {
            seq_t term1 = (seq_t)2 * (seq_t)n * ((seq_t)(n - d));
            seq_t sum   = sum_nd(n, d, 1, d - 1);
            N_d = term1 + (seq_t)2 * sum;
        }
        else
        {
            idx_t L = d - (n - 1);
            if (L < 1) L = 1;
            idx_t R = n - 1;
            seq_t sum = sum_nd(n, d, L, R);
            N_d = (seq_t)2 * sum;
        }

        seq_t nd = (seq_t)d;
        seq_t new_num = num * nd + N_d * den;
        seq_t new_den = den * nd;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
    return num;
}
