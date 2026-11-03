/*
 * A398183  a(n) = second-smallest prime p <= n such that 2n - p is also
 *          prime, or 0 if fewer than two such primes exist.
 * https://oeis.org/A398183
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the second–smallest prime p ≤ n such that 2n – p is
 *            prime, or 0 if fewer than two such primes exist
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9303 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    static uint64_t *primes       = NULL;  /* array of primes ≤ current limit   */
    static size_t   prime_count   = 0;     /* number of primes in the array      */
    static idx_t    current_limit = 0;     /* last limit for which primes were built */

    /* If we have not yet built primes up to n, do so now. */
    if (n > current_limit) {
        /* Free any old prime list. */
        if (primes != NULL)
            free(primes);

        /* Build primes ≤ n.  The helper allocates a new array and returns it
           through the pointer arguments. */
        build_primes_upto(n, &primes, &prime_count);
        current_limit = n;
    }

    /* Find the second smallest prime p ≤ n for which 2n – p is prime. */
    seq_t second_prime = 0;
    size_t matched     = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];          /* p is ≤ n by construction          */
        seq_t candidate = ((seq_t)2 * (seq_t)n) - (seq_t)p;

        /* Test primality of candidate.  The helper works for arbitrary
           precision seq_t values.  For seq_t that happens to fit in 64 bits,
           it automatically calls the native 64‑bit test. */
        if (is_prime_big(candidate)) {
            ++matched;
            if (matched == 2) {
                second_prime = (seq_t)p;
                break;
            }
        }
    }

    return second_prime;   /* 0 if fewer than two matches were found          */
}
