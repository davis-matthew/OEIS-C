/*
 * A277879  Smallest prime r such that p = (q + r + 1)/r, where p = prime(n)
 *          and q = A277878(n).
 * https://oeis.org/A277879
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 86 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A277879 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static uint64_t *prime_cache = NULL;
static size_t     prime_count = 0;     /* number of primes in prime_cache   */
static idx_t      prime_limit = 0;     /* upper numeric limit used          */

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes_at_least(idx_t n)
{
    /* Quickly expand the prime list until the desired index exists. */
    while (prime_count < n) {
        if (prime_limit == 0) prime_limit = 100;
        prime_limit <<= 1;                 /* double the limit          */
        if (prime_cache) free(prime_cache);
        build_primes_upto(prime_limit, &prime_cache, &prime_count);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined below offset   */

    /* Make sure the prime list contains p = prime(n). */
    ensure_primes_at_least(n);
    seq_t p   = (seq_t)prime_cache[n - 1]; /* prime(n)                  */
    seq_t pm1 = p - 1;                     /* p - 1                     */

    /* Iterate over primes r until a suitable one is found. */
    while (1) {
        for (size_t i = 0; i < prime_count; ++i) {
            seq_t r = (seq_t)prime_cache[i];
            seq_t q = r * pm1 - 1;          /* q = r(p-1) - 1            */
            if (isPrimeGeneric(q)) return r;
        }

        /* No suitable r found with the current list; grow it. */
        prime_limit <<= 1;
        if (prime_limit == 0) prime_limit = 2;
        if (prime_cache) free(prime_cache);
        build_primes_upto(prime_limit, &prime_cache, &prime_count);
        /* The loop will retry using the expanded prime list. */
    }
}
