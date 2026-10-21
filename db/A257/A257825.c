/*
 * A257825  Positive integers whose square is the sum of 74 consecutive
 *          squares.
 * https://oeis.org/A257825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7398*a(n-6)-a(n-12) with a(1)=2257, a(2)=2849,
 *            a(3)=21941, a(4)=27713, a(5)=604765, a(6)=763865, a(7)=16669573,
 *            a(8)=21054961, a(9)=162316669, a(10)=205018517,
 *            a(11)=4474051285, a(12)=5651073085
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>   /* for EXIT_FAILURE */

seq_t a(idx_t n)
{
    /* static cache for memoisation (1‑based indexing) */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity (unused index 0) */
    static size_t cache_len  = 0;   /* highest index computed so far */

    if (n == 0)                    /* undefined, return 0 */
        return (seq_t)0;

    /* ensure capacity for indices [0 .. n] (index 0 unused) */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;

        /* first allocation: initialise the 12 known terms */
        if (cache_len == 0) {
            cache[ 1] = 2257ULL;
            cache[ 2] = 2849ULL;
            cache[ 3] = 21941ULL;
            cache[ 4] = 27713ULL;
            cache[ 5] = 604765ULL;
            cache[ 6] = 763865ULL;
            cache[ 7] = 16669573ULL;
            cache[ 8] = 21054961ULL;
            cache[ 9] = 162316669ULL;
            cache[10] = 205018517ULL;
            cache[11] = 4474051285ULL;
            cache[12] = 5651073085ULL;
            cache_len = 12;
        }
    }

    /* compute missing terms only if necessary */
    if ((size_t)n > cache_len) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            seq_t term = (seq_t)7398 * cache[i - 6] - cache[i - 12];
            cache[i] = term;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
