/*
 * A280854  Number of n X 3 0..2 arrays with no element equal to more than one
 *          of its king-move neighbors and with new values introduced in order
 *          0 sequentially upwards.
 * https://oeis.org/A280854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-3) - a(n-4) + 2*a(n-6) - 2*a(n-7) for n
 *            > 9, with initial values: a(1) = 4, a(2) = 29, a(3) = 46, a(4) =
 *            96, a(5) = 256, a(6) = 678, a(7) = 1698, a(8) = 4358, a(9) =
 *            11218
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *a_cache = NULL;          /* array of terms a[0..] (a[0] unused) */
static idx_t   cache_capacity = 0;     /* allocated size of a_cache */
static idx_t   cache_max = 0;          /* largest n for which a_cache[n] is valid */

/* initial values for n=1..9 (a[0] is unused) */
static const seq_t init_values[10] = {
    (seq_t)0,   /* unused, to keep indices 1..9 aligned */
    (seq_t)4,   /* a(1) */
    (seq_t)29,  /* a(2) */
    (seq_t)46,  /* a(3) */
    (seq_t)96,  /* a(4) */
    (seq_t)256, /* a(5) */
    (seq_t)678, /* a(6) */
    (seq_t)1698,/* a(7) */
    (seq_t)4358,/* a(8) */
    (seq_t)11218/* a(9) */
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* value at index 0 is irrelevant */

    /* ensure the cache is allocated large enough for indices up to n */
    if (n + 1 > cache_capacity) {
        idx_t new_capacity = cache_capacity ? cache_capacity * 2 : 16;
        if (new_capacity < n + 1) new_capacity = n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_capacity * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        a_cache = tmp;
        cache_capacity = new_capacity;
    }

    /* compute any missing terms up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        if (k <= 9) {
            a_cache[k] = init_values[(size_t)k];
        } else {
            seq_t val = 0;
            val += (seq_t)2 * a_cache[(size_t)(k - 1)];
            val += (seq_t)4 * a_cache[(size_t)(k - 3)];
            val -= a_cache[(size_t)(k - 4)];
            val += (seq_t)2 * a_cache[(size_t)(k - 6)];
            val -= (seq_t)2 * a_cache[(size_t)(k - 7)];
            a_cache[k] = val;
        }
    }
    cache_max = n;
    return a_cache[(size_t)n];
}
