/*
 * A376579  Least prime p such that p*n+1 is prime, or -1 if no such prime
 *          exists.
 * https://oeis.org/A376579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least prime p such that p*n+1 is prime, or -1 if none The
 *            code uses a prime sieve (build_primes_upto) and is_prime_big to
 *            test primeness of the candidate p*n+1. For odd n the only
 *            possible candidate is p=2, otherwise the search continues over
 *            all primes
 * Range:     i8 n <= 27, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static cache of primes */
static uint64_t *prime_tbl      = NULL; /* array of primes */
static size_t   prime_cnt      = 0;     /* number of primes stored   */
static idx_t    prime_lim      = 0;     /* largest value covered by prime_tbl */

/* Ensure that primes up to @limit are available.
   If the limit is larger than the current cache, rebuild the list
   up to the new limit.  The previous array is freed. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= prime_lim)
        return;

    uint64_t *new_tbl;
    size_t   new_cnt;
    build_primes_upto(limit, &new_tbl, &new_cnt);

    if (prime_tbl)
        free(prime_tbl);

    prime_tbl = new_tbl;
    prime_cnt = new_cnt;
    prime_lim = limit;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;   /* out of range guard */

    /* For odd n the product p*n is odd only when p is even (p=2);
       otherwise p*n+1 is an even number > 2 and composite. */
    if ((n & 1U) != 0) {
        seq_t cand = (seq_t)2 * (seq_t)n + 1;
        if (is_prime_big(cand))
            return 2;
        return (seq_t)-1;
    }

    /* For even n, search primes until a suitable one is found. */
    /* A reasonable initial bound for the prime list.  It is
       large enough to cover all tested values up to n≈10000. */
    const idx_t INITIAL_LIMIT = 2000000ULL;
    _ensure_primes(INITIAL_LIMIT);

    /* First scan the current list. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p   = prime_tbl[i];
        seq_t    cand = (seq_t)p * (seq_t)n + 1;
        if (is_prime_big(cand))
            return (seq_t)p;
    }

    /* If not found, extend the prime list and try again. */
    idx_t new_limit = prime_lim * 2;
    const idx_t MAX_LIMIT = 20000000ULL;   /* upper safety bound */

    while (new_limit <= MAX_LIMIT) {
        _ensure_primes(new_limit);

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p   = prime_tbl[i];
            seq_t    cand = (seq_t)p * (seq_t)n + 1;
            if (is_prime_big(cand))
                return (seq_t)p;
        }

        new_limit *= 2;
    }

    /* No suitable prime found within the searched range. */
    return (seq_t)-1;
}
