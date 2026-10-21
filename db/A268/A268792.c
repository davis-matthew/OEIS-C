/*
 * A268792  Number of n X 2 0..2 arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 51, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268792:
 *  a(1) = 3, a(2) = 22, a(3) = 78, a(4) = 234, a(5) = 652,
 *  a(6) = 1714, a(7) = 4360
 *  For n > 7:
 *      a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) - a(n-6)
 *  The function below computes a(n) for any n ≥ 1.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* cached values */
static idx_t cache_cap = 0;    /* current capacity (size) */
static idx_t cache_len = 0;    /* largest index computed so far */

/* Compute a(n), 1‑based.  idx_t may be uint64_t, seq_t may be any unsigned type. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* ----- initial allocation and base values ----- */
    if (!cache) {
        cache_cap = 8;                                 /* enough for bases */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                  /* allocation failure */

        /* initialise a(0) for 1‑based array (unused) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)78;
        cache[4] = (seq_t)234;
        cache[5] = (seq_t)652;
        cache[6] = (seq_t)1714;
        cache[7] = (seq_t)4360;
        cache_len = 7;
    }

    /* ----- resize if required ----- */
    if ((size_t)n + 1 > cache_cap) {
        idx_t new_cap = (idx_t)((size_t)n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ----- return if already computed ----- */
    if (n <= cache_len) return cache[n];

    /* ----- compute missing terms using the recurrence ----- */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += cache[i-1] * (seq_t)2;
        val += cache[i-2] * (seq_t)3;
        val -= cache[i-3] * (seq_t)2;
        val -= cache[i-4] * (seq_t)6;
        val -= cache[i-5] * (seq_t)4;
        val -= cache[i-6];
        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
