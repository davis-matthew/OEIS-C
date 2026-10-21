/*
 * A252693  Number of (n+2) X (6+2) 0..3 arrays with every consecutive three
 *          elements in every row and column having exactly two distinct
 *          values, and in every diagonal and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 5*a(n-1) - 4*a(n-2) - 11*a(n-3) + 9*a(n-4) + 12*a(n-5)
 *                          + 12*a(n-6) - 34*a(n-7) - 8*a(n-8) + 28*a(n-9) - 8*a(n-10)
 * for n > 11.
 * Initial values (OEIS offset 1):
 * a(1)=1982, a(2)=2719, a(3)=881, a(4)=6246, a(5)=4138
 * a(6)=18940, a(7)=25382, a(8)=82842, a(9)=168598, a(10)=474106
 * a(11)=1144322
 */

#include <stddef.h>  /* for size_t */

#define MAX_N 20000                /* enough for all required n (~10 000) */

static seq_t cache[MAX_N + 1];     /* 1‑based indexing, cache[0] unused */
static idx_t cached_max = 0;       /* largest index that has been computed */

/* initialise the first ten (actually eleven) terms only once */
static void init_cache(void)
{
    cache[1] = 1982ULL;
    cache[2] = 2719ULL;
    cache[3] = 881ULL;
    cache[4] = 6246ULL;
    cache[5] = 4138ULL;
    cache[6] = 18940ULL;
    cache[7] = 25382ULL;
    cache[8] = 82842ULL;
    cache[9] = 168598ULL;
    cache[10] = 474106ULL;
    cache[11] = 1144322ULL;
    cached_max = 11;
}

seq_t a(idx_t n)
{
    /* guard against zero or out‑of‑range requests */
    if (n < 1 || n > MAX_N) {
        /* undefined behaviour for illegal n; return 0 for safety */
        return (seq_t)0;
    }

    /* initialise once on the first call */
    if (cached_max == 0) {
        init_cache();
    }

    /* if the value is already cached, return it immediately */
    if (n <= cached_max) {
        return cache[n];
    }

    /* compute missing terms up to n, using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* i ≥ 12, so the recurrence applies */
        seq_t pos = 5 * cache[i - 1] + 9 * cache[i - 4] +
                    12 * cache[i - 5] + 12 * cache[i - 6] +
                    28 * cache[i - 9];
        seq_t neg = 4 * cache[i - 2] + 11 * cache[i - 3] +
                    34 * cache[i - 7] + 8 * cache[i - 8] +
                    8 * cache[i - 10];
        cache[i] = pos - neg;   /* pos ≥ neg for all valid n */
    }

    cached_max = n;
    return cache[n];
}
