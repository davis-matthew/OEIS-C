/*
 * A387546  Coefficient of x^n in the expansion of ( (1-x+x^5) / (1-x) )^n.
 * https://oeis.org/A387546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} binomial(n,k) * binomial(n-4*k-1, n-5*k)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 5ULL;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* first binomial term */
        seq_t t1 = binomial(n, k);

        /* second binomial parameters */
        idx_t term2_n = n - (4ULL * k + 1ULL);   /* n - 4*k - 1 */
        idx_t term2_k = n - (5ULL * k);          /* n - 5*k */

        /* skip invalid binomial (term2_k > term2_n) */
        if (term2_k > term2_n) continue;

        seq_t t2 = binomial(term2_n, term2_k);

        sum += t1 * t2;
    }
    return sum;
}
