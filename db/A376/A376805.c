/*
 * A376805  Expansion of 1/((1 - x^2)^3 - 9*x)^(1/3).
 * https://oeis.org/A376805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-9)^(n-2k) * binomial(-1/3 , n-2k) * *
 *            binomial(3*n-5*k , k) * The term (-9)^(e)*binomial(-1/3 , e) is
 *            integer for all e≥0. * It satisfies the recurrence * T[0] = 1 *
 *            T[e] = T[e-1] * (9*e-6) / e (e ≥ 1) * where T[e] = (-9)^e *
 *            binomial(-1/3 , e). * This recurrence is used...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Static cache for T[e] = (-9)^e * binomial(-1/3 , e)   */
static seq_t *T_cache = NULL;        /* pointer to cache array */
static idx_t  T_len   = 0;           /* number of elements stored */
static idx_t  T_cap   = 0;           /* allocated capacity */

/* Grow the cache to hold indices up to n   */
static void ensure_T_cache(idx_t n)
{
    if (n >= T_cap) {
        idx_t new_cap = (n + 1);
        seq_t *new_cache = NULL;

        if (T_cache == NULL) {
            new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
            if (new_cache == NULL) abort();    /* allocation failure */
            T_cache = new_cache;
            T_cap   = new_cap;
            T_len   = 0;
        } else {
            new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
            if (new_cache == NULL) abort();
            /* copy old values */
            for (idx_t i = 0; i < T_len; ++i) new_cache[i] = T_cache[i];
            free(T_cache);
            T_cache = new_cache;
            T_cap   = new_cap;
        }

        /* initialize if first time */
        if (T_len == 0) {
            T_cache[0] = (seq_t)1;
            T_len = 1;
        }

        /* compute missing entries */
        for (idx_t e = T_len; e <= n; ++e) {
            /* numerator = T[e-1] * (9*e-6) */
            seq_t tmp = T_cache[e-1] * (seq_t)(9 * e - 6);
            /* division by e is exact */
            T_cache[e] = tmp / (seq_t)e;
        }
        T_len = n + 1;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* we need T[e] for all e up to n */
    ensure_T_cache(n);

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t e = n - 2 * k;                   /* e = n-2k */

        seq_t term1 = T_cache[e];               /* (-9)^e * binomial(-1/3 , e) */
        seq_t term2 = binomial(3 * n - 5 * k, k);

        sum += term1 * term2;
    }

    return sum;
}
