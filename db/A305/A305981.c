/*
 * A305981  Expansion of e.g.f. 1/(1 + LambertW(log(1 - x))).
 * https://oeis.org/A305981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} |Stirling1(n,k)| * k^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using unsigned Stirling numbers of the first kind recurrence
   s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k)  (unsigned s)
   and 0^0 = 1. */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

/* static cache state */
static seq_t *a_cache     = NULL;   /* a(n) values */
static seq_t *pows        = NULL;   /* k^k values */
static seq_t *s_prev      = NULL;   /* Stirling n-1 row */
static seq_t *s_curr      = NULL;   /* Stirling n row   */
static idx_t cache_size   = 0;      /* current array size */
static idx_t max_n_cached = 0;      /* largest n already computed */

/* Ensure the static arrays are large enough for index n */
static void grow_to(idx_t n)
{
    if (cache_size > n) return;
    idx_t old_size = cache_size;
    idx_t new_size = (cache_size == 0) ? 1 : cache_size;
    while (new_size <= n) new_size <<= 1;

    seq_t *new_a  = seq_alloc_new_array(new_size);
    seq_t *new_p  = seq_alloc_new_array(new_size);
    seq_t *new_s1 = seq_alloc_new_array(new_size);
    seq_t *new_s2 = seq_alloc_new_array(new_size);

    /* copy old data */
    for (idx_t i = 0; i < old_size && i < new_size; ++i) {
        new_a[i]  = (a_cache  ? a_cache[i]  : 0);
        new_p[i]  = (pows     ? pows[i]     : 0);
        new_s1[i] = (s_prev   ? s_prev[i]   : 0);
        new_s2[i] = (s_curr   ? s_curr[i]   : 0);
    }
    /* zero new entries */
    for (idx_t i = old_size; i < new_size; ++i) {
        new_a[i]  = 0;
        new_p[i]  = 0;
        new_s1[i] = 0;
        new_s2[i] = 0;
    }

    if (a_cache)  seq_free(a_cache);
    if (pows)     seq_free(pows);
    if (s_prev)   seq_free(s_prev);
    if (s_curr)   seq_free(s_curr);

    a_cache = new_a;
    pows    = new_p;
    s_prev  = new_s1;
    s_curr  = new_s2;
    cache_size = new_size;
}

/* Return the n-th term of A305981 */
seq_t a(idx_t n)
{
    /* grow arrays if needed */
    grow_to(n);

    /* initialise at first call */
    if (max_n_cached == 0 && a_cache[0] == 0) {
        a_cache[0] = (seq_t)1;    /* a(0) */
        s_prev[0]  = (seq_t)1;    /* |s(0,0)| */
        pows[0]    = (seq_t)1;    /* 0^0 */
    }

    /* pre‑compute powers k^k up to n (cache the results) */
    for (idx_t k = 0; k <= n; ++k) {
        if (pows[k] == (seq_t)0) {
            if (k == 0)
                pows[k] = (seq_t)1;
            else
                pows[k] = big_pow((seq_t)k, (seq_t)k);
        }
    }

    /* return cached value if already computed */
    if (n <= max_n_cached)
        return a_cache[n];

    /* compute values for n greater than max_n_cached */
    for (idx_t curr = max_n_cached + 1; curr <= n; ++curr) {
        s_curr[0] = (seq_t)0; /* |s(curr,0)| = 0 for curr > 0 */
        for (idx_t k = 1; k <= curr; ++k) {
            /* unsigned Stirling recurrence:
               s(curr,k) = s(prev,k-1) + (curr-1)*s(prev,k) */
            s_curr[k] = s_prev[k - 1] + ((seq_t)(curr - 1)) * s_prev[k];
        }

        /* a(curr) = sum_{k=1..curr} s_curr[k] * k^k */
        seq_t sum = 0;
        for (idx_t k = 1; k <= curr; ++k)
            sum += s_curr[k] * pows[k];

        a_cache[curr] = sum;

        /* prepare for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    max_n_cached = n;
    return a_cache[n];
}
