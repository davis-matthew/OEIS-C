/*
 * A279677  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 5/4.
 * https://oeis.org/A279677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficients of (1 - x - x^4 + x^5)/(1 + x + x^2 + 2 x^3)
 * Range:     i8 n <= 17, i16 n <= 36, i32 n <= 72, i64 n <= 146,
 *            i128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_capacity = 0;
static idx_t seq_computed = 0; /* number of elements already filled */

static void ensure_seq_cache(idx_t need)
{
    if (seq_cache == NULL) {
        seq_capacity = (need < 16 ? 16 : need + 1);
        seq_cache = (seq_t *)malloc(seq_capacity * sizeof(seq_t));
        seq_computed = 0;
    } else if (need >= seq_capacity) {
        size_t new_cap = seq_capacity * 2;
        if (new_cap < (size_t)(need + 1)) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_seq_cache(n);

    /* initialize the base values once */
    if (seq_computed == 0) {
        seq_cache[0] = 1;
        seq_cache[1] = -2;
        seq_cache[2] = 1;
        seq_computed = 3;
    }

    if (n < seq_computed) {
        return seq_cache[n];
    }

    for (idx_t k = seq_computed; k <= n; ++k) {
        seq_t Nk = 0;
        if (k == 4) Nk = -1;
        else if (k == 5) Nk = 1;
        seq_cache[k] = Nk - seq_cache[k-1] - seq_cache[k-2] - 2 * seq_cache[k-3];
    }

    seq_computed = n + 1;
    return seq_cache[n];
}
