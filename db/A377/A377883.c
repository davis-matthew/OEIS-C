/*
 * A377883  Cogrowth sequence of the 16-element modular group M4(2) = <S,T |
 *          S^8, T^2, STS^3T>.
 * https://oeis.org/A377883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 32*a(n-3) - 40*a(n-4) + 32*a(n-5),
 *            with a(0)=1, a(1)=1, a(2)=1, a(3)=7, a(4)=34, a(5)=126. The
 *            recurrence holds for n>=6. Result is stored in seq_t; if seq_t
 *            is an arbitrary–precision integer type the arithmetic is done
 *            exactly. For built–in unsigned types the values...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* enlarge the cache to hold terms 0..n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;

    /* reallocate to larger size */
    cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (cache == NULL) {
        /* allocation failed – terminate the program */
        abort();
    }

    /* initialize new entries to zero */
    for (size_t i = cache_size; i < needed; ++i)
        cache[i] = (seq_t)0;

    cache_size = needed;
}

/* initialise the first six terms */
static void init_base(void)
{
    if (cache_size >= 6) return;
    ensure_cache(5);   /* need indices 0..5 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)7;
    cache[4] = (seq_t)34;
    cache[5] = (seq_t)126;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* we can safely cast idx_t to size_t because n is small in the tests */
    size_t idx = (size_t)n;

    /* initialise cache if necessary */
    if (cache_size == 0) init_base();
    else if (idx < 6) return cache[idx];

    /* ensure we have space for n */
    ensure_cache(idx);

    /* compute missing terms up to n */
    for (size_t i = 6; i <= idx; ++i) {
        /* recurrence: a(i) = 6*a(i-1) - 14*a(i-2) + 32*a(i-3)
                           - 40*a(i-4) + 32*a(i-5) */
        seq_t t1 = (seq_t)6 * cache[i-1];
        seq_t t2 = (seq_t)14 * cache[i-2];
        seq_t t3 = (seq_t)32 * cache[i-3];
        seq_t t4 = (seq_t)40 * cache[i-4];
        seq_t t5 = (seq_t)32 * cache[i-5];

        /* Perform the computation in the type seq_t.
           For unsigned seq_t the operations are modulo 2^width,
           which is mathematically consistent with the recurrence
           modulo 2^width.  For signed or arbitrary‑precision types
           the exact integer value is produced. */
        cache[i] = t1 - t2 + t3 - t4 + t5;
    }

    return cache[idx];
}
