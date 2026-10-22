/*
 * A395577  1 = Sum_{n>=0} a(n) * x^n * (1 - (4*n+1)*x)^2.
 * https://oeis.org/A395577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n) = 2*(4*n-3)*a(n-1) - (4*n-7)^2*a(n-2)
 *            (n>=2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_cap = 0;  /* current allocation size */
static size_t cache_len = 0;  /* number of valid terms in cache */

/* Ensure the cache can hold at least 'need' elements */
static void
ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the nth term of A395577 */
seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Initialise or extend the cache */
    ensure_cache((size_t)n + 1);

    if (cache_len == 0) {        /* first time: store a(0) and a(1) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* If the desired value is already cached, return it immediately */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Compute missing values up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t a_prev1 = cache[i - 1];
        seq_t a_prev2 = cache[i - 2];

        /* 2*(4*i-3) and (4*i-7)^2 ─ calculated with seq_t arithmetic */
        seq_t i_seq = (seq_t)i;
        seq_t factor1 = (seq_t)2 * ((seq_t)4 * i_seq - (seq_t)3);
        seq_t temp = (seq_t)4 * i_seq - (seq_t)7;
        seq_t factor2 = temp * temp;  /* (4*i-7)^2 */

        /* a(i) = factor1 * a_prev1 - factor2 * a_prev2 */
        cache[i] = factor1 * a_prev1 - factor2 * a_prev2;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
