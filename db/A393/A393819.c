/*
 * A393819  a(n) is the greatest odd semiprime < 2*prime(n).
 * https://oeis.org/A393819
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) = the greatest odd semiprime
 * strictly less than 2·prime(n), where prime(1)=2. It uses a pre‑computed
 * list of primes (via build_primes_upto) and trial factorisation
 * (via factor_u64) to test semiprimality. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* Static storage for the prime table */
static uint64_t * _primes    = NULL;   /* 0‑based array of primes     */
static size_t   _prime_cnt  = 0;       /* number of primes stored     */

/* Ensure that _primes contains at least the first n primes.
   For the expected range (n up to ~10⁴) a fixed upper bound of
   200 000 is adequate. It is built once on the first request. */
static void _ensure_primes(idx_t n)
{
    if (_prime_cnt >= (size_t)n) return;   /* already sufficient */

    /* The upper bound is chosen to contain prime(10000)=104 729. */
    size_t limit = 200000UL;

    if (_prime_cnt == 0) {
        build_primes_upto((idx_t)limit, &_primes, &_prime_cnt);
    }

    /* If the requested n exceeds the pre‑built range, rebuild with a
       larger limit (rare for the test data). */
    if (_prime_cnt < (size_t)n) {
        /* Simple growth strategy: double the limit until it fits. */
        while (_prime_cnt < (size_t)n) {
            limit *= 2U;
            build_primes_upto((idx_t)limit, &_primes, &_prime_cnt);
        }
    }
}

/* Test whether x is an odd semiprime (p·q with p and q odd primes,
 * including the square case p²).  The function returns true only
 * when x is odd and has exactly two prime factors counted with
 * multiplicity. */
static bool _is_odd_semiprime(uint64_t x)
{
    if (x < 3 || (x & 1ULL) == 0ULL) return false;

    uint64_t pf[10];
    uint64_t ex[10];
    size_t   cnt;

    factor_u64(x, pf, ex, &cnt);   /* factorise x into primes pf[i]^ex[i] */

    if (cnt == 1) {
        return (ex[0] == 2ULL);           /* x = p²  */
    }
    if (cnt == 2) {
        return (ex[0] == 1ULL && ex[1] == 1ULL);   /* x = p·q  */
    }
    return false;
}

/* Return a(n) for n >= 3.  For n < 3 the function returns 0
 * (the sequence is defined only from n=3 onwards). */
seq_t a(idx_t n)
{
    if (n < 3) return 0;

    _ensure_primes(n);

    /* 2·prime(n) – since our primes array is 0‑based, prime(n) is _primes[n-1] */
    uint64_t bound = _primes[n - 1] * 2ULL;

    /* Search downward for the first odd semiprime below the bound. */
    for (uint64_t m = bound - 1ULL; m >= 3ULL; --m) {
        if ((m & 1ULL) && _is_odd_semiprime(m))
            return (seq_t)m;
        if (m == 3ULL) break;  /* avoid unsigned wrap‑around */
    }

    return 0;   /* should never happen with the given definition */
}
