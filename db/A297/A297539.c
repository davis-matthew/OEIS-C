/*
 * A297539  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A297539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 11, a(3) = 35, a(4) = 88, a(5) = 461, a(6) =
 *            2050, a(7) = 8057, a(8) = 35640, a(9) = 158090, a(10) = 679629,
 *            a(11) = 2963173, a(12) = 12981912, a(13) = 56560118, a(14) =
 *            246687460 For n > 14 the recurrence holds: a(n) = 3*a(n-1) +
 *            38*a(n-3) - 40*a(n-4) + 7*a(n-5) - 296*a(n-6) + 9...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[14] = {
    (seq_t)1,           (seq_t)11,      (seq_t)35,
    (seq_t)88,          (seq_t)461,     (seq_t)2050,
    (seq_t)8057,        (seq_t)35640,   (seq_t)158090,
    (seq_t)679629,      (seq_t)2963173, (seq_t)12981912,
    (seq_t)56560118,    (seq_t)246687460
};

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused */
static size_t cache_size = 0;    /* allocated size (in entries) */
static idx_t max_computed = 0;   /* highest n with value computed */

static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    /* grow allocation if necessary */
    if (cache_size < (size_t)n + 1) {
        size_t new_size = (size_t)n + 1;
        seq_t *p = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!p) abort();      /* allocation failure */
        cache = p;
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = init[i - 1];
        } else {
            seq_t pos = 0;
            seq_t neg = 0;

            pos += (seq_t)3  * cache[i - 1];
            pos += (seq_t)38 * cache[i - 3];
            pos += (seq_t)7  * cache[i - 5];
            pos += (seq_t)98 * cache[i - 7];
            pos += (seq_t)36 * cache[i - 8];
            pos += (seq_t)388 * cache[i - 9];
            pos += (seq_t)400 * cache[i - 12];
            pos += (seq_t)128 * cache[i - 14];

            neg += (seq_t)40  * cache[i - 4];
            neg += (seq_t)296 * cache[i - 6];
            neg += (seq_t)96  * cache[i - 10];
            neg += (seq_t)104 * cache[i - 11];
            neg += (seq_t)192 * cache[i - 13];

            cache[i] = pos - neg;
        }
    }

    if (n > max_computed) max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
