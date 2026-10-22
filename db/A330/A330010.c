/*
 * A330010  Number of length-n ternary strings x with the property that if w
 *          is a subword of x and |w| >= 3, then w reversed is not a subword
 *          of x.
 * https://oeis.org/A330010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2) for n≥5, a(0)=1, a(1)=3, a(2)=9, a(3)=18,
 *            a(4)=30
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A330010: number of length‑n ternary strings with no palindrome of length >2. */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, abort */

static void grow_cache(idx_t need, seq_t **arr, idx_t *len)
{
    if (*len > need) return;
    idx_t new_len = (*len == 0) ? 1 : *len;
    while (new_len <= need) new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, (size_t)new_len * sizeof(seq_t));
    if (!tmp) abort();
    *arr      = tmp;
    *len      = new_len;
}

seq_t a(idx_t n)
{
    /* Base cases for small n. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)18;
    if (n == 4) return (seq_t)30;

    /* Cache for memoised values. */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;
    static idx_t  last_comp  = (idx_t)-1;  /* no value computed yet */

    /* First free slots for indices 0..4 if needed. */
    if (cache_cap == 0) {
        grow_cache(4, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)30;
        last_comp = 4;
    } else {
        /* Ensure the cache can reach 'n'. */
        grow_cache(n, &cache, &cache_cap);
    }

    /* If we already know a(n), return it. */
    if (n <= last_comp) return cache[n];

    /* Compute missing terms up to 'n' using the recurrence a(i)=a(i-1)+a(i-2). */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2];
    }

    last_comp = n;
    return cache[n];
}
