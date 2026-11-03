/*
 * A358790  a(n) is the least prime p such that (2*n+1)^2 + p^2 is twice a
 *          prime.
 * https://oeis.org/A358790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that ((2*n+1)^2 + p^2)/2 is prime
 * Range:     u8 n <= 6, u16 n <= 568, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9391 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

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

seq_t a(idx_t n)
{
    // Compute (2*n+1)^2 in seq_t.
    seq_t nn   = (seq_t)n;
    seq_t val  = 2 * nn + 1;        // 2*n+1
    seq_t base = val * val;         // (2*n+1)^2

    // Search for the smallest prime p>2 such that ((2*n+1)^2 + p^2)/2 is prime.
    uint64_t p = 3;                 // first odd prime

    for (;;)
    {
        seq_t pp  = (seq_t)p * (seq_t)p;      // p^2
        seq_t sum = base + pp;                // (2*n+1)^2 + p^2
        seq_t q   = sum / 2;                  // (sum)/2, always integer for odd p

        if (isPrimeGeneric(q))
            return (seq_t)p;                  // found smallest prime p

        p = next_prime(p);                    // next prime after current p
    }
}
