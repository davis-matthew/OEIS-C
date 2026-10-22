/*
 * A324358  Total number of occurrences of 8 in the (signed) displacement sets
 *          of all permutations of [n+8] divided by 8!.
 * https://oeis.org/A324358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{j=1..n} (-1)^{j+1} binomial(n,j) * ∏_{t=9}^{n+8-j} t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  ∏_{t=9}^{k} t == k! / 8! .  This avoids large factorials. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* k = n+8-j;   for j=1 we have k = n+7 = current_k.
       prod = ∏_{t=9}^{current_k} t  (i.e. current_k! / 8!).  */
    idx_t current_k = n + 7;
    seq_t prod = 1;

    if (current_k >= 9) {
        for (idx_t t = 9; t <= current_k; ++t)
            prod *= (seq_t)t;
    }

    seq_t sum = 0;

    for (idx_t j = 1; j <= n; ++j) {
        seq_t bin = binomial(n, j);
        seq_t term = prod * bin;

        if ((j & 1) == 1)   /* j odd → positive sign  */
            sum += term;
        else
            sum -= term;

        /* update prod for next j: divide by current_k
           which is the current value of n+8-j.  */
        if (current_k >= 9)
            prod /= current_k;
        else
            prod = 1;

        --current_k;
    }
    return sum;
}
