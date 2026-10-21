/*
 * A276129  a(n) is the number of ordered ways to tile a strip of length n+2
 *          with white tiles of odd lengths summing to length n and two red
 *          squares.
 * https://oeis.org/A276129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 80, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A276129 – number of ordered tilings of length n+2 with two red squares
 *  and odd‑length white tiles summing to n.
 *
 *  The sequence satisfies the linear recurrence
 *      a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6)   for n ≥ 7
 *  with the initial values
 *      a(0)=1, a(1)=3, a(2)=6, a(3)=13, a(4)=27, a(5)=54, a(6)=106.
 */

#include <stdlib.h>

/*  Initial seven values of the sequence. */
static const seq_t _base[7] = {
    (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)13, (seq_t)27, (seq_t)54, (seq_t)106
};

/*  Compute a(n) with memoised recurrence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation array */
    static idx_t  cache_len = 0;  /* number of elements stored */
    static idx_t  cache_cap = 0;  /* allocated capacity */
    idx_t i;

    /*  Return directly for the first seven terms. */
    if (n < 7)
        return _base[n];

    /*  Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (i = 0; i < 7; ++i)
            cache[i] = _base[i];
        cache_len = 7;
    }

    /*  Grow capacity to hold indices up to n. */
    while (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /*  Compute missing values using the recurrence. */
    for (i = cache_len; i <= n; ++i) {
        seq_t val = 3 * cache[i-1];   /* 3*a(i-1) */
        val -= 5 * cache[i-3];        /* -5*a(i-3) */
        val += 3 * cache[i-5];        /* +3*a(i-5) */
        val += cache[i-6];            /* +a(i-6) */
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
