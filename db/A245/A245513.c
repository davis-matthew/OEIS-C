/*
 * A245513  Smallest m such that neither of the two odd numbers that bracket
 *          n^m is a prime.
 * https://oeis.org/A245513
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that neither of the two odd numbers that
 *            * bracket n^m is a prime. * For even n the pair is (n^m-1,
 *            n^m+1); for odd n it is * (n^m-2, n^m+2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 10000 (2026-11-03)
 * Helpers:   ipow_u64_idx, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/ipow_u64_idx.h"
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
    if (n < 2) return 0;   /* offset starts at 2; values below are unused */

    const bool even = (n & 1) == 0;

    for (idx_t m = 1; ; ++m)
    {
        seq_t pow = ipow_u64_idx((uint64_t)n, m);          /* compute n^m */

        seq_t left, right;
        if (even) {
            left  = pow - 1;
            right = pow + 1;
        } else {
            left  = pow - 2;
            right = pow + 2;
        }

        /* If both odd numbers are composite, m is the answer */
        if (!isPrimeGeneric(left) && !isPrimeGeneric(right))
            return (seq_t)m;
    }
}
