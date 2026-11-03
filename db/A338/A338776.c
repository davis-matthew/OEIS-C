/*
 * A338776  a(n) = card(GB(2*n)), where GB(n) is the set of primes which are
 *          Goldbach-associated with n.
 * https://oeis.org/A338776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ prime p : √(2n) < p ≤ n and for every prime q ≤ √(2n),
 *            q ∤ (2n – p) }|
 * Range:     u8 n <= 318, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 101 terms through n = 930 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) uses the definition of Goldbach‑associated primes GB(2n). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* sieve of Eratosthenes returning the prime list up to limit (inclusive) */
static seq_t *build_simple_primes(idx_t limit, size_t *count_out)
{
    if (limit < 2) {
        *count_out = 0;
        return NULL;
    }
    /* boolean array: 1 = prime candidate, 0 = composite */
    idx_t *is_prime = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    if (!is_prime) return NULL;
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }
    /* estimate number of primes: limit / log(limit) + 1 */
    size_t est = (size_t)(limit / (idx_t)(sizeof(double) > 0 ? 1 : 1)) + 1;
    seq_t *primes = mem_new(seq_t, est);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (pos == est) {
                est *= 2;
                primes = mem_resize(seq_t, primes, est);
            }
            primes[pos++] = (seq_t)i;
        }
    }
    free(is_prime);
    *count_out = pos;
    return primes;
}

/* integer square‑root for unsigned 64‑bit values */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid != 0 && mid > x / mid) {       /* mid² > x  (avoid overflow) */
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 2) return 0;

    /* 2n and its integer square root */
    idx_t two_n = n << 1;                /* 2*n  (wraps modulo 2^64, fine for n <= 2^63) */
    idx_t sqrtM = _isqrt_u64(two_n);

    /* Build all primes up to n */
    size_t prime_cnt = 0;
    seq_t *primes = build_simple_primes(n, &prime_cnt);
    if (!primes) return 0;

    /* Find the first prime greater than sqrtM   */
    size_t start = 0;
    while (start < prime_cnt && primes[start] <= sqrtM) ++start;

    seq_t count = 0;
    for (size_t i = start; i < prime_cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        /* p <= n by construction */
        idx_t d = two_n - p;              /* 2n – p   (non‑negative for p ≤ n) */
        bool ok = true;
        for (size_t j = 0; j < start; ++j) {
            idx_t q = (idx_t)primes[j];
            if (d % q == 0) {            /* q divides the product, so reject */
                ok = false;
                break;
            }
        }
        if (ok) ++count;
    }

    mem_free(primes);
    return count;
}
