/*
 * A281229  Smallest number k of the form x^2 + y^2 with 0 <= x <= y such that
 *          gcd(x, y) = 1, x + y = n, and k has no other decompositions into a
 *          sum of two squares.
 * https://oeis.org/A281229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k = x^2 + y^2 with x+y=n, gcd(x,y)=1, * x<=y,
 *            and k has a unique representation (prime if n odd, * 2·prime if
 *            n even). * Implementation scans x from floor(n/2) downwards,
 *            which gives k * values in decreasing order, thus the first valid
 *            pair yields the * minimal k. * Complexity: O(...
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9987 (2026-11-03)
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
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t half = n / 2;
    bool odd = (n & 1) != 0;

    for (idx_t i = half; ; --i)
    {
        idx_t x = i;
        idx_t y = n - x;           /* y >= x because i <= half */

        if (gcd((seq_t)x, (seq_t)y) != 1)
            continue;

        seq_t s = (seq_t)x * (seq_t)x + (seq_t)y * (seq_t)y;

        if (odd)
        {
            if (isPrimeGeneric(s))
                return s;
        }
        else
        {
            if ((s & (seq_t)1) != 0)   /* s should be even */
                continue;
            seq_t q = s >> 1;         /* q = s / 2 */
            if ((q % 4) != 1)
                continue;
            if (isPrimeGeneric(q))
                return s;             /* s = 2*q */
        }

        if (i == 0)
            break;                     /* should not happen for n>=3 */
    }

    /* Conjecture guarantees existence; return 0 as a fallback. */
    return (seq_t)0;
}
