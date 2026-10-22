/*
 * A390136  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x/(1+x)) ).
 * https://oeis.org/A390136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-1)^{n-k} * (n+1)^{k-1} *
 *            binomial(n-1,n-k) / k! (a(0)=1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 11, i64 n <= 18, i128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: For 64‑bit seq_t we compute all intermediate values in
 * __int128 to avoid overflow; for arbitrary‑precision seq_t we
 * rely on the provided big‑int utilities. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;
static idx_t   cache_cap = 0;
static idx_t   cache_size = 0;

static void ensure_cache(idx_t need)
{
    if (need < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_buf = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
    cache = new_buf;
    cache_cap = new_cap;
}

static seq_t compute_term(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* If seq_t fits in 64 bits we use __int128 for safety */
    if (sizeof(seq_t) <= 8) {
        __int128 sum  = 0;
        __int128 pow  = 1;
        for (idx_t i = 0; i < n - 1; ++i) pow *= (n + 1);

        __int128 binom = 1;
        __int128 fall  = 1;
        int     sign   = 1;

        for (idx_t j = 0; j < n; ++j) {
            __int128 term = pow * binom * fall;
            if (sign < 0) term = -term;
            sum += term;

            if (j + 1 < n) {
                pow   = pow / (n + 1);
                binom = binom * (n - 1 - j) / (j + 1);
                fall  = fall * (n - j);
                sign  = -sign;
            }
        }
        return (seq_t)sum;
    } else {
        /* Use the arbitrary‑precision utilities */
        seq_t pow  = big_pow((seq_t)(n + 1), (seq_t)(n - 1));
        seq_t binom = 1;
        seq_t fall  = 1;
        seq_t sum   = 0;
        int    sign = 1;

        for (idx_t j = 0; j < n; ++j) {
            seq_t term = pow * binom * fall;
            if (sign < 0) term = -term;
            sum += term;

            if (j + 1 < n) {
                pow   = pow / (seq_t)(n + 1);
                binom = binom * (seq_t)(n - 1 - j) / (seq_t)(j + 1);
                fall  = fall * (seq_t)(n - j);
                sign  = -sign;
            }
        }
        return sum;
    }
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        idx_t need = n + 1;
        ensure_cache(need);
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = compute_term(i);
        }
        cache_size = n + 1;
    }
    return cache[n];
}
