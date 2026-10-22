/*
 * A304921  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A304921
 *  Empirical recurrence (valid for n>9):
 *  a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3) + 2*a(n-4)
 *          - 2*a(n-5) - 7*a(n-6) - 6*a(n-7)
 *  Initial values:
 *  a(1)=4, a(2)=5, a(3)=15, a(4)=31, a(5)=73,
 *  a(6)=191, a(7)=466, a(8)=1125, a(9)=2762
 *
 *  The function computes the sequence for any n ≥ 1, memoizing
 *  partial results so that subsequent calls, in any order, are fast.
 *
 *  All arithmetic is performed in seq_t; only +, -, *, / are used.
 */

#include <stdlib.h>

static seq_t init[10] = {0, 4, 5, 15, 31, 73, 191, 466, 1125, 2762};

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* largest index computed; cache[0] unused */

/* Ensure cache contains values up to n, computing missing ones */
static void
ensure_up_to(idx_t n)
{
    if (cache_size >= n)
        return;

    /* allocate or grow cache */
    size_t new_len = (size_t)n + 1;            /* because cache[0] unused */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache == NULL)
            return;                    /* allocation failure; caller will fail */
    } else if (new_len > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return;                    /* allocation failure */
        cache = tmp;
    }

    /* Initialize known values up to 9 */
    if (cache_size <= 9) {
        for (idx_t i = (cache_size < 1) ? 1 : cache_size; i <= ((new_len > 10) ? 9 : new_len - 1); ++i)
            cache[i] = init[i];
    }

    /* Compute remaining terms using the recurrence */
    idx_t start = (cache_size < 10) ? 10 : cache_size + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = cache[i - 1]
                  + (cache[i - 2] << 1)          /* 2*cache[i-2] */
                  + (cache[i - 3] << 2)          /* 4*cache[i-3] */
                  + (cache[i - 4] << 1)          /* 2*cache[i-4] */
                  - (cache[i - 5] << 1)          /* -2*cache[i-5] */
                  - (cache[i - 6] * 7)           /* -7*cache[i-6] */
                  - (cache[i - 7] * 6);          /* -6*cache[i-7] */
        cache[i] = val;
    }

    cache_size = n;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* behaviour below the offset is irrelevant */

    ensure_up_to(n);
    return cache[n];
}
