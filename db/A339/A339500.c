/*
 * A339500  Maximum length of sequences of prime numbers that start with
 *          prime(n) and are arithmetic progressions of common difference less
 *          than prime(n).
 * https://oeis.org/A339500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = maximum length of an arithmetic progression of primes
 * starting at prime(n) with common difference < prime(n).  The function
 * searches all possible differences d < prime(n) (skipping odd d when
 * prime(n) > 2, since all primes > 2 are odd) and, for each d, counts
 * consecutive primes in the form prime(n)+k*d until a composite is
 * encountered.  The largest count found is returned. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Cached primes up to a certain limit */
static uint64_t *cached_primes   = NULL;
static size_t   cached_cnt      = 0;
static idx_t    cached_limit    = 0;

/* Rebuild the prime table so that it contains at least n primes.
 * The function expands the table if needed; it never shrinks it. */
static void rebuild_primes_up_to(idx_t n)
{
    if (n == 0) return;
    /* Compute an upper bound that is guaranteed to contain at least n primes */
    idx_t limit = _prime_limit_for_idx(n);
    if (limit <= (idx_t)cached_limit) return;   /* already sufficient */

    /* Free old table and build a new one */
    if (cached_primes) free(cached_primes);
    size_t cnt;
    build_primes_upto(limit, &cached_primes, &cnt);
    cached_cnt  = cnt;
    cached_limit = limit;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety: OEIS offset starts at 1 */

    /* Ensure we have the first n primes */
    rebuild_primes_up_to(n);

    uint64_t p0_u64 = cached_primes[n - 1];
    seq_t   p0      = (seq_t)p0_u64;          /* starting prime */

    seq_t best = 1;                          /* at least the starting prime */

    for (seq_t d = 1; d < p0; ++d) {
        /* For primes > 2, only even differences can produce an odd AP */
        if (p0 > (seq_t)2 && (d & (seq_t)1)) continue;

        seq_t len     = 1;
        seq_t current = p0;
        while (1) {
            current += d;
            if (!isPrimeGeneric(current)) break;
            ++len;
        }
        if (len > best) best = len;
    }
    return best;
}
