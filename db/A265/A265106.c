/*
 * A265106  Expansion of (x^5-x^4-2*x^3+x^2-x)/(-x^4+x^3-2*x^2+3*x-1).
 * https://oeis.org/A265106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) - a(n-4)   (n ≥ 6)
 * Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=6, a(4)=16, a(5)=36
 *
 * Also a(n) = 2*a(n-1) + a(n-3) + 2      (n ≥ 5)
 * The function uses memoisation so any order of calls works.
 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* dynamic cache of computed terms   */
static size_t seq_cache_size = 0;    /* allocated size of seq_cache       */

seq_t a(idx_t n)
{
    /* Small table of the first six terms */
    static const seq_t init[6] = {0, 1, 2, 6, 16, 36};

    /* Initialise cache on first use */
    if (!seq_cache) {
        /* Allocate a reasonably sized buffer; at least 8 entries */
        size_t init_size = 8;
        seq_cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!seq_cache) return 0;          /* allocation failure */
        seq_cache_size = init_size;
        for (size_t i = 0; i < 6; ++i)
            seq_cache[i] = init[i];
    }

    /* Return early for the stored initial terms */
    if (n < 6)
        return seq_cache[(size_t)n];

    /* Ensure the cache is large enough */
    size_t up = (size_t)n + 1;          /* we need indices 0 .. n inclusive */
    if (seq_cache_size < up) {
        size_t new_size = seq_cache_size ? seq_cache_size * 2 : 8;
        if (new_size < up) new_size = up;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        seq_cache = tmp;
        seq_cache_size = new_size;
    }

    /* Compute missing terms up to n using the 4‑term recurrence */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        seq_t v = 3 * seq_cache[i - 1];
        v -= 2 * seq_cache[i - 2];
        v += seq_cache[i - 3];
        v -= seq_cache[i - 4];
        seq_cache[i] = v;
    }

    return seq_cache[(size_t)n];
}
