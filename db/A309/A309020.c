/*
 * A309020  Expansion of x * Product_{k>=0} (1 + x^(2^k) + x^(2^(k+1)) -
 *          x^(2^(k+2))).
 * https://oeis.org/A309020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = a(n); a(2*n+1) = a(n) + a(n+1) -
 *            a(n-1)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values */
static unsigned char *computed = NULL; /* flags whether a value is filled */
static size_t cache_cap = 0;

/* Ensure the cache can hold at least indices up to [need] */
static void ensure_capacity(idx_t need) {
    if ((size_t)need < cache_cap) return;
    size_t new_cap = need * 2 + 10;
    seq_t *new_c = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *new_f = (unsigned char *)realloc(computed, new_cap);
    if (!new_c || !new_f) { /* allocation failed; abort */
        abort();
    }
    cache = new_c;
    computed = new_f;
    for (size_t i = cache_cap; i < new_cap; ++i) computed[i] = 0;
    cache_cap = new_cap;
}

/* Recursively compute a(n) with memoization */
static seq_t a_rec(idx_t n) {
    ensure_capacity(n);
    if (computed[n]) return cache[n];

    seq_t value;
    if (n == 0) {
        value = 0;
    } else if (n == 1) {
        value = 1;
    } else if ((n & 1) == 0) {          /* even */
        value = a_rec(n >> 1);           /* a(n) = a(n/2) */
    } else {                            /* odd, n >= 3 */
        idx_t m = n >> 1;                /* m = floor((n-1)/2) */
        seq_t a_m    = a_rec(m);
        seq_t a_m1   = a_rec(m + 1);
        seq_t a_m_1  = a_rec(m - 1);
        value = a_m + a_m1 - a_m_1;
    }

    cache[n]    = value;
    computed[n] = 1;
    return value;
}

/* External function requested by the build */
seq_t a(idx_t n) {
    return a_rec(n);
}
