/*
 * A331529  a(n) is the number of nonnegative integers that can be represented
 *          in a 7-segment display by using only n segments (version A006942).
 * https://oeis.org/A331529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 43, u64 n <= 84, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A331529
 * Recurrence: a(n) = a(n-2)+a(n-3)+a(n-4)+3*a(n-5)+3*a(n-6)+a(n-7)  for n>13
 * Initial values:
 *   a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=5, a(6)=7,
 *   a(7)=12, a(8)=19, a(9)=33, a(10)=59, a(11)=99, a(12)=170, a(13)=290
 * The function caches results for fast repeated calls.  It is O(n) time and
 * uses O(n) space.  It works for any unsigned seq_t and idx_t
 * (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128, etc.). */

#include <stdlib.h>   /* malloc, realloc, abort */

static const seq_t init_vals[14] = {
    (seq_t)0,  (seq_t)0,  (seq_t)1,  (seq_t)1,
    (seq_t)2,  (seq_t)5,  (seq_t)7,  (seq_t)12,
    (seq_t)19, (seq_t)33, (seq_t)59, (seq_t)99,
    (seq_t)170,(seq_t)290
};

static seq_t *cache = NULL;          /* dynamic array of computed values */
static size_t  cache_size = 0;       /* number of elements allocated */
static idx_t   highest_computed = 13;/* index of the highest computed value */

/* Prepare cache up to index n and compute any missing values. */
static void ensure_and_compute(idx_t n)
{
    /* Grow cache if necessary */
    if (cache_size <= n) {
        /* Allocate enough space (at least n+1 or 14) */
        size_t new_size = (n + 1 > 14 ? n + 1 : 14);
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = (seq_t *)tmp;
        cache_size = new_size;
        /* Initialize the first 14 values if we just allocated the cache */
        if (cache_size == new_size && new_size >= 14) {
            for (idx_t i = 0; i < 14; ++i)
                cache[i] = init_vals[i];
            highest_computed = 13;
        }
    }

    /* Compute missing terms */
    for (idx_t i = highest_computed + 1; i <= n; ++i) {
        seq_t v = cache[i - 2] + cache[i - 3] + cache[i - 4];
        v += (seq_t)3 * cache[i - 5];
        v += (seq_t)3 * cache[i - 6];
        v += cache[i - 7];
        cache[i] = v;
    }
    highest_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 14)          /* Return hard‑coded first 14 terms */
        return init_vals[n];

    ensure_and_compute(n);
    return cache[n];
}
