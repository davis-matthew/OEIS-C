/*
 * A300843  Number of 4-cycles in the n-transposition graph.
 * https://oeis.org/A300843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * n! * ( 2 * binomial(n,3) + binomial(n,4) ) / 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We cache factorials so that repeated calls are fast; 
    binomials are computed directly from the provided binomial
    routine.  All operations are done in seq_t.  The routine
    works for any size of seq_t (including arbitrary‑precision). */

#include <stdlib.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* values for n = 1,2 are 0 */
    if (n < 3)
        return 0;

    /* ----   Cache factorials (static)   ---- */
    static seq_t *fac_cache = NULL;
    static idx_t  fac_last  = 0;          /* highest n for which fac_cache[n] is set */

    if (fac_cache == NULL) {
        fac_cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        fac_cache[0] = 1;                /* 0! = 1 */
        fac_last   = 0;
    }

    if (n > fac_last) {
        /* grow cache to include n */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        memcpy(new_cache, fac_cache, sizeof(seq_t) * (fac_last + 1));

        for (idx_t k = fac_last + 1; k <= n; ++k)
            new_cache[k] = new_cache[k - 1] * (seq_t)k;  /* factorial recurrence */

        free(fac_cache);
        fac_cache = new_cache;
        fac_last  = n;
    }

    /* ----   compute the binomials   ---- */
    seq_t binom3 = binomial(n, 3);        /* for n < 3 binomial returns 0 */
    seq_t binom4 = binomial(n, 4);        /* for n < 4 binomial returns 0 */

    /* temp = 2 * binom3 + binom4 */
    seq_t temp = binom3 * (seq_t)2;
    temp      += binom4;

    /* a(n) = 3 * n! * temp / 4 */
    seq_t result = fac_cache[n];
    result *= (seq_t)3;
    result *= temp;
    result /= (seq_t)4;   /* division is exact */

    return result;
}
