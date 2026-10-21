/*
 * A256803  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 0 and no row sum 0 or 1 and
 *          no column sum 0 or 1.
 * https://oeis.org/A256803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (empirical linear recurrence):
        a(1) = 32, a(2) = 86, a(3) = 237, a(4) = 641,
        a(5) = 1731, a(6) = 4690, a(7) = 12707, a(8) = 34408
        a(n) = a(n-1)+a(n-2)+7*a(n-3)+7*a(n-4)+3*a(n-5)
               -a(n-6)-6*a(n-7)-2*a(n-8)   (for n ≥ 9)
    This recurrence is derived from the OEIS entry for A256803.   */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static const seq_t base[8] = {
    32ULL,    /* a(1) */
    86ULL,    /* a(2) */
    237ULL,   /* a(3) */
    641ULL,   /* a(4) */
    1731ULL,  /* a(5) */
    4690ULL,  /* a(6) */
    12707ULL, /* a(7) */
    34408ULL  /* a(8) */
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored (a(1)..a(cache_len)) */
static size_t cache_cap = 0;   /* allocated capacity of cache */

static void
ensure_cache_size(size_t n)
{
    if (cache_cap < n) {
        size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_len < n) {
        size_t start = cache_len;
        if (start < 8) {
            /* fill the initial fixed terms */
            for (size_t i = start; i < 8 && i < n; ++i) {
                cache[i] = base[i];
            }
        }

        /* compute the remaining terms using the recurrence */
        size_t i = (cache_len > 8 ? cache_len : 8);
        for (; i < n; ++i) {
            cache[i] = cache[i-1] + cache[i-2]
                     + cache[i-3] * 7
                     + cache[i-4] * 7
                     + cache[i-5] * 3
                     - cache[i-6]
                     - cache[i-7] * 6
                     - cache[i-8] * 2;
        }
        cache_len = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, safe default */
    size_t need = (size_t)n;        /* n fits in size_t for expected use */
    ensure_cache_size(need);
    return cache[n-1];               /* array is 0‑based, n starts at 1 */
}
