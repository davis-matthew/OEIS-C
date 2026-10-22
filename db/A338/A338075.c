/*
 * A338075  Diagonal terms in the expansion of (1+x*y*z)/(1-x-y-z).
 * https://oeis.org/A338075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = T(n‑1) + T(n) (n > 0) where T(k) = binomial(3k,
 *            k) * binomial(2k, k) (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* works for any seq_t / idx_t defined by the build. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *T_cache = NULL;   /* cache of T(k) values */
static idx_t  T_len   = 0;      /* number of values stored (0 … T_len‑1) */
static size_t T_cap   = 0;      /* allocated capacity           */

/* Allocate or extend the cache so that indices 0 … n are available. */
static void ensure_T(idx_t n)
{
    if (n < T_len) return;            /* nothing to do */

    size_t needed = n + 1;
    if (needed > T_cap) {
        size_t new_cap = T_cap ? T_cap * 2 : 4;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(T_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;             /* allocation failed – ignore (as per build policy) */
        T_cache = tmp;
        T_cap = new_cap;
    }

    for (idx_t k = T_len; k <= n; ++k) {
        seq_t b1 = binomial(3 * k, k);
        seq_t b2 = binomial(2 * k, k);
        T_cache[k] = b1 * b2;
    }
    T_len = n + 1;
}

/* Required function: compute the n‑th term of A338075. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */
    ensure_T(n);
    return T_cache[n - 1] + T_cache[n];
}
