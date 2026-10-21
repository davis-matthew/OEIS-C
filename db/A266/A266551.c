/*
 * A266551  Image of n under the 3p+1 map, which is a variation of the 3x+1
 *          (Collatz) map.
 * https://oeis.org/A266551
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n is prime then 3*n+1 else n /
 *            smallest_prime_factor(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static bool _primes_inited = false;
static uint64_t *_primes = NULL;
static size_t _primes_cnt = 0;

/* initialise a small sieve of primes up to 1 000 000 */
static void _init_primes(void) {
    if (!_primes_inited) {
        build_primes_upto(1000000, &_primes, &_primes_cnt);
        _primes_inited = true;
    }
}

/* test whether n is a prime using trial division by the pre‑computed primes */
static bool _is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 1; i < _primes_cnt; ++i) {          /* primes[0] = 2, already handled */
        uint64_t p = _primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* return the smallest prime divisor of n (returns n itself if n is prime) */
static uint64_t _spf_uint64(uint64_t n) {
    if (n < 2) return n;
    if (n % 2 == 0) return 2;
    for (size_t i = 1; i < _primes_cnt; ++i) {          /* primes[0] = 2, already handled */
        uint64_t p = _primes[i];
        if (p * p > n) break;
        if (n % p == 0) return p;
    }
    return n;   /* n is prime */
}

/* OEIS sequence A266551 */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* behaviour below the offset is irrelevant */

    _init_primes();

    seq_t val = (seq_t)n;

    if (_is_prime_uint64((uint64_t)n))
        return val * 3 + 1;
    else {
        uint64_t spf = _spf_uint64((uint64_t)n);
        return val / spf;
    }
}
