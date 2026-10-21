/*
 * A296315  Number of n X 3 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A296315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 6*a(n-3) + 3*a(n-4) - 3*a(n-6) +
 *            a(n-7) - 3*a(n-9) + a(n-11) for n >= 12 initial values (a(1) ..
 *            a(11)): 3, 11, 32, 96, 295, 902, 2747, 8380, 25577, 78021,
 *            238029 (a(0) is unused) The recurrence is derived empirically
 *            from the OEIS sequence. This implementation uses mem...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* stores computed values 1 .. N */
static idx_t cache_size = 0;  /* current allocated size (>=1) */

static const seq_t base[12] = {
    (seq_t)0,   /* a(0) dummy */
    (seq_t)3,   /* a(1) */
    (seq_t)11,  /* a(2) */
    (seq_t)32,  /* a(3) */
    (seq_t)96,  /* a(4) */
    (seq_t)295, /* a(5) */
    (seq_t)902, /* a(6) */
    (seq_t)2747,/* a(7) */
    (seq_t)8380,/* a(8) */
    (seq_t)25577,/* a(9) */
    (seq_t)78021,/* a(10) */
    (seq_t)238029/* a(11) */
};

static void ensure_cache(idx_t n)
{
    if (n < 12)
        return;

    if (cache_size < 12) {
        /* allocate initial array up to 11 */
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return;   /* allocation failure is ignored (UB) */
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = base[i];
    }

    if (cache_size <= n) {
        idx_t new_size = n + 1;          /* need space up to n */
        seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_ptr) return;            /* ignore failure (UB) */
        cache = new_ptr;
        idx_t old_size = cache_size;
        cache_size = new_size;

        /* compute missing terms from old_size up to n */
        for (idx_t i = old_size; i <= n; ++i) {
            /* positive contributions */
            seq_t pos = cache[i-1];
            pos += cache[i-2] * (seq_t)4;
            pos += cache[i-3] * (seq_t)6;
            pos += cache[i-4] * (seq_t)3;
            pos += cache[i-7];
            pos += cache[i-11];
            /* negative contributions */
            seq_t neg = cache[i-6] * (seq_t)3;
            neg += cache[i-9] * (seq_t)3;
            cache[i] = pos - neg;
        }
    }
}

seq_t a(idx_t n)
{
    /* handle small n directly using base array */
    if (n <= 11)
        return base[n];

    /* otherwise use memoised recurrence */
    ensure_cache(n);
    return cache[n];
}
