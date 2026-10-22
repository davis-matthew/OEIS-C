/*
 * A379932  E.g.f. A(x) satisfies A(x) = ( exp(-x) + x*A(x) )^2.
 * https://oeis.org/A379932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,  b(0) = 1
   for n ≥ 1:
     b(n) = n * a(n‑1) + (–1)^n
     a(n) = Σ_{i=0..n} C(n,i) · b(i) · b(n‑i)
   The sequence a(n) is OEIS A379932. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static caches for a(n) and b(n) */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static idx_t  cache_cap = 0;          /* allocated capacity */
static idx_t  last_computed = (idx_t)-1; /* highest n computed (-1 = none) */

/* enlarge the cached arrays so that they can hold indices up to n   */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;
    idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
    while (new_cap <= n)
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* make sure the cache is large enough */
    ensure_cache(n);

    /* initialise the first term if this is the first call */
    if (last_computed == (idx_t)-1) {
        a_cache[0] = (seq_t)1;
        b_cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* compute all terms up to n that have not yet been computed */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        /* b(k) = k * a(k-1) + (–1)^k */
        seq_t t = (seq_t)k * a_cache[k - 1];
        if (k & 1)          /* odd k : add –1 */
            t -= (seq_t)1;
        else                /* even k : add +1 */
            t += (seq_t)1;
        b_cache[k] = t;

        /* a(k) = Σ_{i=0..k} C(k,i) * b(i) * b(k-i) */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(k,0) */
        for (idx_t i = 0; i <= k; ++i) {
            sum += binom * b_cache[i] * b_cache[k - i];
            if (i < k) {
                binom = binom * (seq_t)(k - i);
                binom = binom / (seq_t)(i + 1);   /* exact division */
            }
        }
        a_cache[k] = sum;
    }
    last_computed = n;
    return a_cache[n];
}
