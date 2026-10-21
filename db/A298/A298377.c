/*
 * A298377  Number of n X 3 0..1 arrays with every element equal to 0, 2, 3, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 6*a(n-3) - 16*a(n-4) + 6*a(n-5) +
 *            23*a(n-6) + 16*a(n-7) - 13*a(n-8) - 40*a(n-9) - 50*a(n-10) +
 *            14*a(n-11) + 106*a(n-12) + 53*a(n-13) - 18*a(n-14) - 38*a(n-15)
 *            - 34*a(n-16) - 8*a(n-17) for n > 18. Initial values: a(1)=1,
 *            a(2)=7, a(3)=6, a(4)=18, a(5)=30, a(6)=87, a(7)=...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_len = 0;     /* number of terms currently stored */

/* 19 initial terms of the sequence */
static const seq_t initial[19] = {
    1, 7, 6, 18, 30, 87, 200, 522, 1421, 3805, 10578,
    29465, 82498, 232593, 656577, 1858073, 5265011,
    14930674, 42368708
};

/* Initialize the cache with the first 19 terms. */
static void init_cache(void)
{
    if (cache_len > 0)
        return;  /* already initialized */

    cache_len = sizeof(initial) / sizeof(initial[0]); /* 19 */
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    for (size_t i = 0; i < cache_len; ++i)
        cache[i] = initial[i];
}

/* Extend the cache so that it contains at least n terms (1‑based). */
static void extend_cache(idx_t n)
{
    if (n == 0)
        return;
    if (cache_len >= (size_t)n)
        return;

    size_t new_len = (size_t)n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute terms from cache_len up to new_len-1 (0‑based indices). */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* We are computing a(i+1).  For i >= 19 it uses the recurrence. */
        if (i < 19) {
            /* Terms 1..19 are already stored in `initial`; no action needed. */
            continue;
        }
        cache[i] = 3*cache[i-1] + 3*cache[i-2] - 6*cache[i-3]
                   - 16*cache[i-4] + 6*cache[i-5] + 23*cache[i-6]
                   + 16*cache[i-7] - 13*cache[i-8] - 40*cache[i-9]
                   - 50*cache[i-10] + 14*cache[i-11] + 106*cache[i-12]
                   + 53*cache[i-13] - 18*cache[i-14] - 38*cache[i-15]
                   - 34*cache[i-16] - 8*cache[i-17];
    }
    cache_len = new_len;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    init_cache();
    extend_cache(n);
    return cache[n-1];
}
