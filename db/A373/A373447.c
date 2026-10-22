/*
 * A373447  Number of ternary strings of length n avoiding the substrings 00,
 *          11, 22, 121, 212, 202.
 * https://oeis.org/A373447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4) - a(n-6); initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=14, a(5)=23, a(6)=36
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* Cached values a(0) ... a(k) */
static idx_t cached_len = 0;       /* Number of cached values (must be >=1) */

/* Ensure that cache has at least n+1 entries, computing any missing terms. */
static void ensure_cache(idx_t n)
{
    idx_t required = n + 1;
    if (cached_len >= required) return;          /* already have enough */

    /* Allocate or grow cache */
    size_t new_len = (size_t)required;
    if (cache == NULL) {
        /* First allocation: at least 7 entries to hold initial values */
        if (new_len < 7) new_len = 7;
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!cache) exit(1); /* allocation failed */
        cached_len = 0;
    } else {
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }

    /* Fill initial terms if not already filled */
    if (cached_len <= 0) {
        cache[0] = (seq_t)1;       /* a(0) */
        cache[1] = (seq_t)3;       /* a(1) */
        cache[2] = (seq_t)6;       /* a(2) */
        cache[3] = (seq_t)9;       /* a(3) */
        cache[4] = (seq_t)14;      /* a(4) */
        cache[5] = (seq_t)23;      /* a(5) */
        cache[6] = (seq_t)36;      /* a(6) */
        cached_len = 7;
    }

    /* Compute any missing terms using the recurrence */
    for (size_t i = (size_t)cached_len; i < new_len; ++i)
    {
        /* For i >= 7 use recurrence; for i < 7 values already set */
        if (i < 7) continue;   /* initial values already filled */

        /* a(i) = a(i-2) + 2*a(i-3) + a(i-4) - a(i-6) */
        seq_t val = cache[i-2];
        val = val + cache[i-3];
        val = val + cache[i-3];
        val = val + cache[i-4];
        val = val - cache[i-6];
        cache[i] = val;
    }

    cached_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
