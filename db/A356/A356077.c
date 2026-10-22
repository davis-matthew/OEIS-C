/*
 * A356077  a(n) is the least prime p such that 2*n - p^2 is prime, or -1 if
 *          there is no such p.
 * https://oeis.org/A356077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least prime p such that 2*n - p^2 is prime, or -1 if none
 * Range:     i8 n <= 8581, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a sparse table of primes up to sqrt(2*n) and repeated primality tests. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64(idx_t n) */
#include "../utils/build_primes_upto.h"  /* build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt) */
#include "../utils/is_prime.h"           /* is_prime(idx_t n) */

/* cache of primes, up to prime_limit.  Maintained across calls. */
static uint64_t *prime_list = NULL;   /* primes in ascending order */
static size_t prime_cnt   = 0;
static idx_t prime_limit  = 0;

/* Ensure that primes are available up to at least m (inclusive). */
static void ensure_primes_upto(idx_t m)
{
    if (m <= prime_limit) return;   /* already covered */

    if (prime_list) free(prime_list);
    build_primes_upto(m, &prime_list, &prime_cnt);
    prime_limit = m;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;   /* out of defined range */

    /* 2*n must be computed in idx_t to avoid overflow in seq_t. */
    idx_t two_n = (idx_t)2 * n;

    /* Largest prime whose square is < 2*n. */
    idx_t m = _floor_isqrt_u64(two_n - 1);

    ensure_primes_upto(m);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)m) break;           /* beyond needed primes */

        __uint128_t sq = (__uint128_t)p * p;
        if (sq >= (__uint128_t)two_n) continue;   /* 2*n - p^2 <= 0 */

        idx_t t = two_n - (idx_t)sq;          /* 2*n - p^2, positive */

        if (t >= 2 && is_prime(t)) {
            return (seq_t)p;                  /* found the least such prime */
        }
    }

    return (seq_t)-1;                       /* no prime found */
}
