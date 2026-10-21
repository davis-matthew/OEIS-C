/*
 * A261118  Expansion of psi(x)^2 * psi(-x^3)^2 / (phi(-x^4) * psi(-x^6)) in
 *          power of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A261118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261118
 *
 * Sequence A261118 is defined by the Euler transform of the
 * 24‑periodic sequence b(d) = [2, -2, 0, 0, 2, -1, 2, -1,
 *   0, -2, 2, -2, 2, -2, 0, -1, 2, -1, 2, 0, 0, -2, 2, -2].
 *
 * The recurrence used is:
 *
 *   a(0) = 1
 *   a(n) = (1/n) * Σ_{k=1..n} c(k) * a(n-k)     (n ≥ 1)
 *
 *   where c(k) = Σ_{d|k} d * b(d)
 *
 * This implementation caches values of a(n) and c(n) so each
 * value is computed at most once.  The divisor sum for c(k)
 * is done in O(√k) time, giving an overall O(n√n) algorithm,
 * which is fast enough for n≈10000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* 24‑periodic sequence b(d) for d ≥ 1 */
static const int b24[24] = {
    2,  -2, 0, 0,  2, -1, 2, -1,
    0, -2, 2, -2, 2, -2, 0, -1,
    2, -1, 2, 0,  0, -2, 2, -2
};

/* cached terms a(n) and divisor sums c(n) */
static seq_t   *cache = NULL;      /* a(n) values, cache[0] = 1 */
static size_t   cache_len = 0;     /* number of cached a(n) values */

static int64_t *carr  = NULL;      /* c(n) values (signed) */
static size_t   carr_len = 0;      /* number of cached c(n) values */

/* compute the sequence value a(n) */
seq_t a(idx_t n)
{
    /* base case a(0) = 1 */
    if (n == 0) {
        if (cache_len == 0) {
            cache = (seq_t*)malloc(sizeof(seq_t));
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* ensure space for n+1 entries */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;

        /* resize cache for a(n) */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;

        /* resize carr for c(n) */
        int64_t *tmpc = (int64_t*)realloc(carr, new_len * sizeof(int64_t));
        if (!tmpc) return (seq_t)0;        /* allocation failed */
        carr = tmpc;

        /* initialise new entries and fill missing c(k) */
        for (size_t i = carr_len; i < new_len; ++i) carr[i] = 0;

        /* compute c(k) for all k from carr_len to n */
        for (size_t k = carr_len; k < new_len; ++k) {
            int64_t ck = 0;
            /* iterate over divisors of k */
            for (uint64_t d = 1; (__int128)d * d <= k; ++d) {
                if (k % d == 0) {
                    int val_d   = b24[(d - 1) % 24];
                    ck += (int64_t)d * (int64_t)val_d;
                    uint64_t other = k / d;
                    if (other != d) {
                        int val_o = b24[(other - 1) % 24];
                        ck += (int64_t)other * (int64_t)val_o;
                    }
                }
            }
            carr[k] = ck;
        }
        carr_len = new_len;

        /* compute a(s) for s = cache_len .. n */
        for (size_t s = cache_len; s < new_len; ++s) {
            __int128 sum = 0;
            for (size_t k = 1; k <= s; ++k) {
                sum += (__int128)carr[k] * (__int128)cache[s - k];
            }
            cache[s] = (seq_t)(sum / (__int128)s);   /* division is exact */
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
