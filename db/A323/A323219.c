/*
 * A323219  a(n) = [x^n] (1 - 4*x)^(-n/2)*x/(1 - x).
 * https://oeis.org/A323219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in (1 - 4*x)^(-n/2) * x/(1-x)
 *            = Σ_{k=0}^{n-1} binom(n/2 + k - 1, k) * 4^k
 * Recurrence: a_n = a_{n-1} + ... (n‑dependent), but the direct sum
 * with the product recurrence
 *    T_0 = 1
 *    T_k = T_{k-1} * (2n + 4k - 4) / k
 *    a_n = Σ_{k=0}^{n-1} T_k   (a_0 = 0)
 * Implementation is fully integer and memoised.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised results */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;      /* allocated number of elements      */
    static idx_t   max_n = 0;      /* largest index computed so far     */

    /* Quick answer for n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Allocate or grow cache to hold index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* allocation failed – return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise cache[0] if it was never set */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)0;

    /* Compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sum;
        if (i == 0) {
            sum = (seq_t)0; /* a(0) = 0 */
        } else {
            seq_t term = (seq_t)1;  /* T_0 */
            sum = term;            /* contribution of k = 0 */

            /* Build terms T_k iteratively */
            for (idx_t k = 1; k < i; ++k) {
                /* numerator = 2*n + 4*k - 4 */
                seq_t num = (seq_t)2 * i + (seq_t)4 * k - (seq_t)4;

                /* Use 128‑bit intermediate to avoid overflow when the
                 * intermediate product exceeds the width of seq_t.
                 * The division by k is exact. */
                __uint128_t prod = (__uint128_t)term * (__uint128_t)num;
                term = (seq_t)(prod / (idx_t)k);

                sum += term;
            }
        }
        cache[i] = sum;
    }

    max_n = n;
    return cache[n];
}
