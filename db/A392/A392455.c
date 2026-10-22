/*
 * A392455  a(n) = Sum_{k=0..floor(2*n/5)} binomial(2*k+1,2*n-5*k).
 * https://oeis.org/A392455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) + a(n-5) - 2*a(n-6) + a(n-7), a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=3, a(4)=1, a(5)=1, a(6)=10
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 97, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 0 … cache_len-1 already computed */
static idx_t cache_len = 0;

/* Compute a(n) using the recurrence, memoising all intermediate
   values in a dynamically sized array.  The function is independent
   of the order of calls. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call with the first seven terms. */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)10;
    }

    /* If the requested index is already known, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate index n. */
    idx_t new_len = n + 1;         /* n is at least cache_len, so new_len>cache_len */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;                   /* realloc cannot fail for the small sizes we use */

    /* Fill in missing values using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = cache[i-3] * (seq_t)4;
        seq_t term2 = cache[i-5];
        seq_t term3 = cache[i-6] * (seq_t)2;
        seq_t term4 = cache[i-7];
        seq_t val   = term1 + term2 + term4;  /* add positive terms first */
        val -= term3;                         /* then subtract */
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
