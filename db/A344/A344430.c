/*
 * A344430  a(n) = Sum_{k=1..n} mu(k) * k^k.
 * https://oeis.org/A344430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * k^k
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mobius.h"   /* seq_t mobius(idx_t)          */
#include "../utils/big_pow.h"  /* seq_t big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    static idx_t     computed_up_to = 0;
    static seq_t   *sums           = NULL;
    static size_t   cap            = 0;          /* allocated size     */

    if (n < 1)                                 /* offset is 1          */
        return 0;

    /* Grow the array to at least index n */
    if (n > computed_up_to) {
        size_t required = (size_t)n + 1;         /* 1‑based, ignore 0 */
        if (required > cap) {
            sums = (seq_t *)realloc(sums, required * sizeof(seq_t));
            cap  = required;
        }
        if (computed_up_to == 0 && cap > 0)     /* initialise sums[0] */
            sums[0] = 0;

        /* Compute terms from the first missing index up to n */
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            seq_t mu   = mobius(k);              /* -1,0,1  */
            seq_t pk   = big_pow((seq_t)k, (seq_t)k); /* k^k   */
            seq_t term = mu * pk;
            sums[k] = sums[k - 1] + term;
        }
        computed_up_to = n;
    }

    return sums[n];
}
