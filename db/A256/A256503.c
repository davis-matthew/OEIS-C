/*
 * A256503  Smallest k>=1 such that n^2 + (n+1)^2 + ... + (n+k)^2 is prime or
 *          a(n)=0 if there is no such k.
 * https://oeis.org/A256503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that Σ_{i=0}^{k} (n+i)^2 is prime;
 *            computed via closed forms: - if 2n^2+2n+1 is prime → k = 1 -
 *            else if 3n^2+6n+5 is prime → k = 2 - else if 6n^2+30n+55 is
 *            prime → k = 5 - otherwise a(n) = 0 (Offset is 1: a(1) is the
 *            first term.)
 * Range:     u8 n <= 7, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    if (n == 0) return 0;          /* offset starts at 1, but catch 0 safely */

    seq_t nn = (seq_t)n;

    seq_t p1 = 2 * nn * nn + 2 * nn + 1;
    if (isPrimeGeneric(p1)) return (seq_t)1;

    seq_t p2 = 3 * nn * nn + 6 * nn + 5;
    if (isPrimeGeneric(p2)) return (seq_t)2;

    seq_t p3 = 6 * nn * nn + 30 * nn + 55;
    if (isPrimeGeneric(p3)) return (seq_t)5;

    return (seq_t)0;
}
