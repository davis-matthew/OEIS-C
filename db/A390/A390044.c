/*
 * A390044  Expansion of 1 / ((1-x)^4 - x^5).
 * https://oeis.org/A390044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5) Initial
 *            values: a(0)=1, a(1)=4, a(2)=10, a(3)=20, a(4)=35
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Memoised sequence storage  */
static seq_t *cache   = NULL;   /* points to an array of seq_t */
static size_t cache_len = 0;    /* number of elements actually stored  */
static size_t cache_cap = 0;    /* allocated capacity               */

seq_t a(idx_t n)
{
    /* ----- initialise first five values once ----- */
    if (cache_len == 0) {
        const seq_t init[5] = { (seq_t)1, (seq_t)4, (seq_t)10, (seq_t)20, (seq_t)35 };
        cache_cap = 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* fatal error – leave caller to handle */
            return (seq_t)0;
        }
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    /* ----- grow cache capacity if required ----- */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* fatal error – leave caller to handle */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ----- compute missing terms up to n ----- */
    while (cache_len <= (size_t)n) {
        /* recurrence uses the last five stored values */
        seq_t a0 = cache[cache_len - 5];
        seq_t a1 = cache[cache_len - 4];
        seq_t a2 = cache[cache_len - 3];
        seq_t a3 = cache[cache_len - 2];
        seq_t a4 = cache[cache_len - 1];

        /* a(n) = 4*a4 - 6*a3 + 4*a2 - a1 + a0 */
        seq_t val = (seq_t)(4 * a4 - 6 * a3 + 4 * a2 - a1 + a0);

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[(size_t)n];
}
