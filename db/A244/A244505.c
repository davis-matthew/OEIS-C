/*
 * A244505  Greater of twin primes of (40n-23,40n-21).
 * https://oeis.org/A244505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greater of the twin primes (40k-23, 40k-21) for the
 *            nth k that makes both numbers prime. a(n) = 40*k - 21 where k is
 *            the nth positive integer such that isPrime(40k-23) &&
 *            isPrime(40k-21)
 * Range:     u8 n <= 2, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9842 (2026-11-03)
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined offset, return 0 for safety */

    seq_t count = 0;
    idx_t k = 1;

    while (true) {
        idx_t p1 = 40ULL * k - 23;
        idx_t p2 = 40ULL * k - 21;

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2)) {
            ++count;
            if (count == n) {
                return (seq_t)p2;  /* greater of the twin primes */
            }
        }
        ++k;
    }
}
