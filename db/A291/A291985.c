/*
 * A291985  Expansion of 1/((1+x)*(1+x^2)*(1+x^3)*(1+x^4)*(1+x^5)).
 * https://oeis.org/A291985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1.
 * The generating function satisfies (1+x)(1+x²)(1+x³)(1+x⁴)(1+x⁵) · A(x) = 1.
 * Expanding the first factor gives a polynomial
 * P(x)=∑_{k=0}^{15} p_k x^k   (p_0=1).
 *  Then the coefficients satisfy for n>0
 *      a(n) = -∑_{k=1}^{15} p_k · a(n-k)    (with a(m)=0 for m<0).
 * The p_k are integers with |p_k| ≤ 32 and can be pre‑computed once.
 * The implementation uses a small cache so repeated calls are O(1) after
 * the first required value.  The algorithm is O(n·15) per new value. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_pk(void);

/* coefficients of P(x)=∏_{k=1}^5(1+x^k)  (degree 15) */
static int pk[16];
static int pk_initialized = 0;

/* cache of computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* initialise the coefficients pk[0..15] once */
static void init_pk(void) {
    int p[16] = {0};
    p[0] = 1;
    for (int k = 1; k <= 5; ++k) {
        int tmp[16] = {0};
        for (int i = 0; i <= 15; ++i) {
            if (p[i] == 0) continue;
            tmp[i] += p[i];
            if (i + k <= 15)
                tmp[i + k] += p[i];
        }
        memcpy(p, tmp, sizeof p);
    }
    memcpy(pk, p, sizeof pk);
    pk_initialized = 1;
}

/* recompute the cache up to and including value n */
static void recompute_up_to(size_t n) {
    /* allocate new cache */
    seq_t *new_cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!new_cache) abort();

    /* initial value a(0)=1 */
    new_cache[0] = 1;

    /* compute values using the recurrence */
    for (size_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 15; ++k) {
            if (i >= (size_t)k) {
                sum += (seq_t)pk[k] * new_cache[i - k];
            }
        }
        new_cache[i] = -sum;  /* p0 = 1 */
    }

    /* free old cache */
    free(cache);
    cache = new_cache;
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    /* initialise pk once */
    if (!pk_initialized) init_pk();

    if (n < 0) return 0;  /* undefined for negative indices */

    /* Ensure cache covers requested index */
    if (cache == NULL || (size_t)n >= cache_len) {
        recompute_up_to((size_t)n);
    }
    return cache[(size_t)n];
}
