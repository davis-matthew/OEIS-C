/*
 * A260881  Number of trapezoidal words of length n.
 * https://oeis.org/A260881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 * + Sum_{i=1..n} (n-i+1) * phi(i) * +
 *            Sum_{i=0..floor((n-4)/2)} 2*(n-2*i-3) * phi(i+2) * where phi is
 *            the Euler totient function. * This implementation precomputes
 *            phi values up to the * largest n requested and reuses them for
 *            subsequent calls
 * Range:     u8 n <= 11, u16 n <= 74, u32 n <= 3044, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9821 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* compute phi up to 'limit', increasing the cached array as needed */
static void _ensure_phi_upto(idx_t limit, seq_t **phi_cache, idx_t *cache_len)
{
    if (*cache_len > limit) return;                 /* already sufficient */

    seq_t *new_cache = mem_resize(seq_t, *phi_cache, (limit + 1));
    if (!new_cache) {                               /* allocation failed */
        *phi_cache = NULL;
        *cache_len = 0;
        return;
    }
    *phi_cache = new_cache;

    /* initialise with φ(n) = n */
    for (idx_t i = 0; i <= limit; ++i)
        (*phi_cache)[i] = (seq_t)i;

    /* sieve for φ */
    for (idx_t p = 2; p <= limit; ++p) {
        if ((*phi_cache)[p] == (seq_t)p) {          /* p is prime */
            seq_t sp = (seq_t)p;
            for (idx_t j = p; j <= limit; j += p) {
                (*phi_cache)[j] = (*phi_cache)[j] -
                                  (*phi_cache)[j] / sp;
            }
        }
    }

    *cache_len = limit + 1;
}

seq_t a(idx_t n)
{
    /* static cache of φ values */
    static seq_t *phi_cache = NULL;
    static idx_t  cache_len = 0;

    /* ensure φ is computed up to n */
    _ensure_phi_upto(n, &phi_cache, &cache_len);
    if (!phi_cache) return (seq_t)0;  /* fallback if memory failed */

    seq_t result = 1;  /* initial constant term */

    /* first sum: Σ_{i=1..n} (n-i+1) * φ(i) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)(n - i + 1);
        result += mult * phi_cache[i];
    }

    /* second sum: Σ_{i=0..floor((n-4)/2)} 2*(n-2*i-3)*φ(i+2) */
    if (n >= 4) {
        idx_t max_i = (n - 4) / 2;
        for (idx_t i = 0; i <= max_i; ++i) {
            seq_t factor = 2 * (seq_t)(n - 2 * i - 3);
            result += factor * phi_cache[i + 2];
        }
    }

    return result;
}
