/*
 * A346751  Expansion of e.g.f. log( 1 + x^3 * exp(x) / 3! ).
 * https://oeis.org/A346751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = binomial(n,3) - (1/n) * Sum_{k=1..n-1} binomial(n,k) *
 *            binomial(n-k,3) * k * a(k)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0; a(1) = 0; a(2) = 0 */

#include <stdlib.h>
#include <stdint.h>

/* forward declaration of helper functions from utils, if needed
   (none used in this implementation) */

/* Static cache for computed terms */
static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;   /* allocated size */
static idx_t a_cache_computed = 0;   /* highest index computed + 1 */

/* Ensure the cache can hold at least (n+1) elements */
/* Allocate or grow as needed, preserving already computed values */
static void ensure_a_cache(idx_t n) {
    idx_t need = n + 1;
    if (need <= a_cache_cap) return;
    /* Double the size until large enough */
    idx_t new_cap = a_cache_cap ? a_cache_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* Allocation failure – in a real system you might abort */
        exit(1);
    }
    if (a_cache) {
        for (idx_t i = 0; i < a_cache_computed; ++i) {
            new_arr[i] = a_cache[i];
        }
        free(a_cache);
    }
    a_cache = new_arr;
    a_cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;   /* a(0)=a(1)=a(2)=0 */

    ensure_a_cache(n);

    /* Compute missing terms if needed */
    if (a_cache_computed <= n) {
        for (idx_t i = a_cache_computed; i <= n; ++i) {
            if (i < 3) {
                a_cache[i] = (seq_t)0;
                continue;
            }
            /* binomial(i,3) = i*(i-1)*(i-2)/6 */
            seq_t binom_i3 = ((seq_t)i * (i - 1) * (i - 2)) / (seq_t)6;

            seq_t sum = (seq_t)0;
            seq_t binom_ik = (seq_t)1;  /* binom(i,0) */
            for (idx_t k = 1; k < i; ++k) {
                /* Update binom(i,k) = binom(i,k-1) * (i-k+1) / k */
                binom_ik = (binom_ik * (seq_t)(i - k + 1)) / (seq_t)k;

                idx_t rem = i - k;
                if (rem >= 3) {
                    /* binom(rem,3) = rem*(rem-1)*(rem-2)/6 */
                    seq_t binom_rem3 = ((seq_t)rem * (rem - 1) * (rem - 2)) / (seq_t)6;
                    seq_t term = binom_ik;
                    term = term * binom_rem3;
                    term = term * (seq_t)k;
                    term = term * a_cache[k];
                    sum += term;
                }
                /* else binom(rem,3) == 0, term contributes 0 */
            }
            /* a(i) = binom_i3 - sum / i */
            a_cache[i] = binom_i3 - sum / (seq_t)i;
        }
        a_cache_computed = n + 1;
    }

    return a_cache[n];
}
