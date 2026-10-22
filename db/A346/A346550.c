/*
 * A346550  Expansion of Sum_{k>=0} k! * x^k * (1 + x)^(k+1).
 * https://oeis.org/A346550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)*a(n-1) + 2*(n-1)*a(n-2) + (n-2)*a(n-3) Initial
 *            values: a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values */
static size_t cache_len = 0;  /* number of valid entries */
static size_t cache_cap = 0;  /* allocated capacity */

seq_t a(idx_t n) {
    /* Ensure cache holds at least n+1 values */
    if (cache_len <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;    /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In the unlikely event realloc fails we just abort here */
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;

        /* Compute new entries from cache_len up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)4;
            } else {
                /* Recurrence: a(i) = (i-2)*a(i-1) + 2*(i-1)*a(i-2) + (i-2)*a(i-3) */
                seq_t t1 = ((seq_t)(i - 2)) * cache[i - 1];
                seq_t t2 = ((seq_t)2) * ((seq_t)(i - 1)) * cache[i - 2];
                seq_t t3 = ((seq_t)(i - 2)) * cache[i - 3];
                cache[i] = t1 + t2 + t3;
            }
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
