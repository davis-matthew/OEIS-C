/*
 * A264508  Number of (2+1)X(n+1) arrays of permutations of 0..n*3+2 with each
 *          element having directed index change -1,1 0,-1 0,1 or 1,0.
 * https://oeis.org/A264508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 2*a(n-1) + 7*a(n-2) - 8*a(n-3) -
 *            9*a(n-4) + 10*a(n-5) - 1*a(n-6) + 1*a(n-8) -10*a(n-9) +
 *            9*a(n-10) + 8*a(n-11) -7*a(n-12) -2*a(n-13) +1*a(n-14) initial
 *            values: a(1)=2, a(2)=2, a(3)=14, a(4)=38, a(5)=137, a(6)=432,
 *            a(7)=1417, a(8)=4554, a(9)=14710, a(10)=47394, a(11)=15...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The cache is indexed from 1.  cache[i] holds a(i). */
static seq_t *cache        = NULL;   /* array 1 .. N */
static idx_t cache_size    = 0;        /* allocated entries (1 .. cache_size-1) */
static idx_t computed      = 0;        /* last index that has been computed */

/* Ensure the cache can hold indices up to n, and that the first 14 terms are present. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate enough space for the first 14 terms. */
        cache_size = (n > 14) ? n + 1 : 15;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL) return;          /* allocation failed */
        /* Known initial terms (index 0 is unused) */
        static const seq_t init[15] = {
            0,          /* unused 0 */
            2, 2, 14, 38, 137, 432, 1417,
            4554, 14710, 47394, 152762, 492284,
            1586369, 5112128
        };
        for (idx_t i = 1; i <= 14 && i < cache_size; ++i)
            cache[i] = init[i];
        computed = 14;    /* we have computed up to a(14) */
    } else if (cache_size <= n) {
        /* Grow the cache to accommodate n. */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return;          /* realloc failed */
        /* Zero the new area. */
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined, but harmless */

    ensure_cache(n);
    if (cache == NULL) return 0;            /* allocation failed */

    if (n <= computed)                      /* already known */
        return cache[n];

    /* Compute a(k) for k = computed+1 .. n.  Valid from 15 upwards. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val = 0;
        val += cache[k - 1] * (seq_t)2;
        val += cache[k - 2] * (seq_t)7;
        val -= cache[k - 3] * (seq_t)8;
        val -= cache[k - 4] * (seq_t)9;
        val += cache[k - 5] * (seq_t)10;
        val -= cache[k - 6] * (seq_t)1;
        val += cache[k - 8] * (seq_t)1;
        val -= cache[k - 9] * (seq_t)10;
        val += cache[k -10] * (seq_t)9;
        val += cache[k -11] * (seq_t)8;
        val -= cache[k -12] * (seq_t)7;
        val -= cache[k -13] * (seq_t)2;
        val += cache[k -14] * (seq_t)1;
        cache[k] = val;
    }
    computed = n;
    return cache[n];
}
