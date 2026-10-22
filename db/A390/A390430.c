/*
 * A390430  Least prime index of n plus greatest prime index of n; a(1) = 0.
 * https://oeis.org/A390430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = min_prime_index(n) + max_prime_index(n)
 * Range:     u8 n <= 718, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

static bool _prepared = false;
static idx_t _max_n = 0;
static size_t _prime_cnt = 0;
static uint64_t *_primes = NULL;
static idx_t *_prime_idx = NULL;          /* _prime_idx[p] = index of prime p (1‑based) */

static void _prepare(idx_t n)
{
    /* If we already have enough primes, do nothing. */
    if (_prepared && n <= _max_n)
        return;

    /* Rebuild primes up to n. */
    free(_primes);
    free(_prime_idx);

    build_primes_upto(n, &_primes, &_prime_cnt);

    _prime_idx = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    memset(_prime_idx, 0, (n + 1) * sizeof(idx_t));
    for (size_t i = 0; i < _prime_cnt; ++i) {
        idx_t p = _primes[i];
        if (p <= n)
            _prime_idx[p] = (idx_t)(i + 1);     /* indices start at 1 */
    }

    _max_n  = n;
    _prepared = true;
}

seq_t a(idx_t n)
{
    if (n <= 1)           /* a(1) = 0, a(0) is irrelevant */
        return 0;

    _prepare(n);          /* build prime tables up to n */

    idx_t min_idx = (idx_t)-1;   /* largest unsigned value */
    idx_t max_idx = 0;

    idx_t m = n;                      /* copy for factorisation */
    for (size_t i = 0; i < _prime_cnt && _primes[i] <= m / _primes[i]; ++i) {
        uint64_t p = _primes[i];
        if (m % p == 0) {
            idx_t idx = _prime_idx[p];
            if (idx < min_idx) min_idx = idx;
            if (idx > max_idx) max_idx = idx;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1) {                      /* remaining prime factor */
        idx_t idx = _prime_idx[m];
        if (idx < min_idx) min_idx = idx;
        if (idx > max_idx) max_idx = idx;
    }

    /* Sum of the least and greatest prime indices. */
    seq_t result = (seq_t)min_idx + (seq_t)max_idx;
    return result;
}
