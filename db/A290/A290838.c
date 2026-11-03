/*
 * A290838  a(n) = smallest prime p such that 2p - 2n + 1 is prime.
 * https://oeis.org/A290838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that 2p-2n+1 is prime
 * Range:     u8 n <= 228, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 9922 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Compute the nth term of OEIS A290838.
 * a(n) is the smallest prime p > n for which 2p-2n+1 is also prime. */
seq_t a(idx_t n)
{
    for (idx_t m = 1;; ++m) {                 /* iterate over potential p = n+m */
        seq_t p = (seq_t)n + (seq_t)m;        /* candidate prime p */
        if (!isPrimeGeneric(p))
            continue;                        /* p must be prime */

        seq_t q = 2 * (seq_t)m + (seq_t)1;    /* 2p-2n+1 = 2m+1 */
        if (isPrimeGeneric(q))
            return p;                        /* found the smallest suitable prime */
    }
}
