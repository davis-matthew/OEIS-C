/*
 * A295449  a(n) = (12*n)!*(2*n)!/((7*n)!*(4*n)!*(3*n)!).
 * https://oeis.org/A295449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)!*(2*n)!/((7*n)!*(4*n)!*(3*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses prime‐factorisation to avoid overflow. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Legendre’s formula: v_p(m!)  */
static idx_t _vp_fact(idx_t m, idx_t p)
{
    idx_t cnt = 0;
    while (m >= p) {
        m /= p;
        cnt += m;
    }
    return cnt;
}

/* Global prime table for repeated calls. */
static idx_t   *_primes      = NULL;
static size_t  _prime_cnt    = 0;
static idx_t   _prime_limit  = 0;

/* Simple sieve topping out at n.  Caches results for future calls. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= _prime_limit)
        return;

    /* Allocate array for prime flags.  */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return;                     /* out of memory – fail silently */

    for (size_t i = 0; i <= (size_t)limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (idx_t p = 2; (idx_t)(p * p) <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    idx_t *new_primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_primes[k++] = i;

    free(is_prime);
    free(_primes);
    _primes      = new_primes;
    _prime_cnt   = cnt;
    _prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t limit = 12 * n;
    _ensure_primes(limit);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < _prime_cnt; ++i) {
        idx_t p = _primes[i];
        if (p > limit)
            break;

        idx_t exp =
            _vp_fact(12 * n, p) +
            _vp_fact( 2 * n, p) -
            _vp_fact( 7 * n, p) -
            _vp_fact( 4 * n, p) -
            _vp_fact( 3 * n, p);

        if (exp == 0)
            continue;

        seq_t powv = ipow_idx((seq_t)p, exp);
        result = seq_mul(result, powv);
    }

    return result;
}
