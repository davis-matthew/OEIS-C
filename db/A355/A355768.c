/*
 * A355768  E.g.f. satisfies A(x)^(A(x)^2) = 1 + x*A(x).
 * https://oeis.org/A355768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (n-2*k+1)^{k-1} * Stirling1(n,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where Stirling1 are signed Stirling numbers of the first kind   */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static cache of computed values (a[0]..a[max_computed]) */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static idx_t max_computed = 0;

    /* static storage for the previous Stirling row: s_prev[k] == s(cur-1,k) */
    static seq_t *s_prev     = NULL;
    static size_t s_prev_len = 0;

    /* first call: initialise */
    if (cache == NULL) {
        cache      = (seq_t *)calloc(1, sizeof(seq_t));
        cache[0]   = (seq_t)1;          /* a(0) = 1 */
        cache_len  = 1;
        max_computed = 0;

        s_prev     = (seq_t *)malloc(sizeof(seq_t));
        s_prev[0]  = (seq_t)1;          /* s(0,0) = 1 */
        s_prev_len = 1;
    }

    /* if already computed, return immediately */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute rows up to n */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        size_t len   = (size_t)cur + 1;                 /* row length */
        seq_t *s_next = (seq_t *)malloc(len * sizeof(seq_t));
        /* compute Stirling row using recurrence:
           s(cur,k) = s(cur-1,k-1) - (cur-1)*s(cur-1,k)        */
        for (size_t k = 0; k <= cur; ++k) {
            seq_t term1 = (k >= 1 && k-1 < s_prev_len) ? s_prev[k-1] : (seq_t)0;
            seq_t term2 = (k < s_prev_len) ? s_prev[k] : (seq_t)0;
            s_next[k] = term1 - (seq_t)(cur - 1) * term2;
        }

        /* compute a(cur) from the row */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= cur; ++k) {
            seq_t base = (seq_t)cur - (seq_t)2 * (seq_t)k + (seq_t)1;
            seq_t exp  = (seq_t)(k - 1);
            seq_t pow_val = big_pow(base, exp);
            sum += s_next[k] * pow_val;
        }

        /* resize cache if necessary */
        if ((size_t)cur >= cache_len) {
            size_t new_len = cache_len ? cache_len << 1 : 1;
            while (new_len <= (size_t)cur) new_len <<= 1;
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            cache_len = new_len;
        }

        cache[cur] = sum;
        max_computed = cur;

        /* free previous row and move to the next */
        free(s_prev);
        s_prev     = s_next;
        s_prev_len = len;
    }

    return cache[n];
}
