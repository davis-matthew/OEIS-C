/*
 * A362478  E.g.f. satisfies A(x) = exp(x + x^3/3 * A(x)^3).
 * https://oeis.org/A362478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (1/3)^k * (3k+1)^(n-2k-1) / (k! *
 *            (n-3k)!) Computed by iterating over k, using the recurrence
 *            B_{k+1} = B_k * (n-3k)(n-3k-1)(n-3k-2)/(k+1) with B_0 = 1, then
 *            dividing B_k by 3^k before multiplying by (3k+1)^(n-2k-1). The
 *            use of division early keeps 64‑bit intermediate...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   _binom_div_by_3k, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/_binom_div_by_3k.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)0;
    idx_t max_k = n / 3;
    seq_t binom = (seq_t)1;           /* B_0 = 1 */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t binom_div = _binom_div_by_3k(binom, k);
        idx_t exp = n - 2 * k - 1;
        seq_t pow_base = big_pow((seq_t)(3 * k + 1), (seq_t)exp);
        seq_t term = binom_div * pow_base;
        result += term;

        if (k == max_k)            /* no need to update for next k */
            break;

        /* Update binom for next k (B_{k+1}) */
        seq_t mul = (seq_t)(n - 3 * k);
        mul *= (seq_t)(n - 3 * k - 1);
        mul *= (seq_t)(n - 3 * k - 2);
        binom = binom * mul / (seq_t)(k + 1);
    }

    return result;
}
