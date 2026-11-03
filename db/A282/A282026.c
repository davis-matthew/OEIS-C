/*
 * A282026  a(n) is the smallest m with gcd(m, 2*n+1) = 1 such that 2*n + 2*m
 *          + 1 is composite.
 * https://oeis.org/A282026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m | gcd(m, 2*n+1)=1 and 2*n + 2*m + 1 is composite }
 * Range:     u8 n <= 2144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9292 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/gcd.h"
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
    seq_t p = (seq_t)n * 2 + 1;       // 2*n + 1
    idx_t m = 1;
    for (;; ++m) {
        seq_t sm = (seq_t)m;
        if (gcd(sm, p) == 1) {
            seq_t q = p + sm * 2;    // 2*n + 2*m + 1
            if (!isPrimeGeneric(q))
                return sm;
        }
    }
}
