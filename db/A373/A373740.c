/*
 * A373740  Expansion of e.g.f. exp(x^2/2 * (1 + x)).
 * https://oeis.org/A373740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373740
   Formula: a(n) = n! * Σ_{k=0..⌊n/2⌋} binomial(k,n-2k)/(2^k * k!).
   Recurrence: a(0)=1, a(1)=0, a(2)=1,
               a(n) = ((n-1) * (2*a(n-2) + 3*(n-2)*a(n-3))) / 2   for n≥3
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values 0..cache_last */
static idx_t  cache_last = 2;    /* highest n that is already computed */

/* Ensure the cache buffer is big enough to hold indices 0..need. */
static void ensure_cache_size(idx_t need)
{
    idx_t required = (need > 2 ? need : 2);   /* at least 2 for base cases */

    if (cache == NULL) {
        cache = (seq_t*)malloc((required + 1) * sizeof(seq_t));
        if (cache == NULL) return;           /* allocation failure – not expected */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        /* cache_last remains 2 until computed values are produced */
    } else if (cache_last < required) {
        cache = (seq_t*)realloc(cache, (required + 1) * sizeof(seq_t));
        if (cache == NULL) return;
        for (idx_t i = cache_last + 1; i <= required; ++i)
            cache[i] = (seq_t)0;
        /* cache_last is not modified here – it will be updated after filling */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    ensure_cache_size(n);

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term2  = cache[i - 2];
        seq_t term3  = cache[i - 3];
        seq_t inner  = (seq_t)2 * term2
                     + (seq_t)3 * (seq_t)(i - 2) * term3;
        seq_t val    = (seq_t)(i - 1) * inner;
        val = val / (seq_t)2;                     /* exact division */
        cache[i] = val;
    }
    cache_last = n;
    return cache[n];
}
