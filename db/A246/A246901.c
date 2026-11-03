/*
 * A246901  a(n) is the n-th smallest prime p such that p+4n is also prime.
 * https://oeis.org/A246901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th smallest prime p such that p+4*n is also prime
 * Range:     u8 n <= 12, u16 n <= 852, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 899 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple brute‑force search with the supplied primality test
   isPrimeGeneric(seq_t). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
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

/* Return the n‑th term (n ≥ 1).  If n == 0 the function returns 0
   but the offset for this sequence is 1 so that case is unused. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not used – offset is 1 */

    idx_t found = 0;
    idx_t p = 2;                      /* start with the smallest prime */

    /* Search for primes in ascending order. */
    while (1) {
        if (isPrimeGeneric((seq_t)p)) {
            /* Check the second condition: p + 4*n must also be prime. */
            seq_t target = (seq_t)p + (seq_t)n * ((seq_t)4);
            if (isPrimeGeneric(target)) {
                ++found;
                if (found == n)
                    return (seq_t)p;
            }
        }

        /* Advance to the next candidate prime.  Skip even numbers after 2. */
        if (p == 2)
            p = 3;
        else
            p += 2;                   /* only odd numbers are checked */
    }
}
