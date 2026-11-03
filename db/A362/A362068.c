/*
 * A362068  a(n) is the smallest positive integer k such that n can be
 *          expressed as the arithmetic mean of k squares.
 * https://oeis.org/A362068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8473 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest k such that n can be expressed as the mean of k squares
   (i.e., n*k is a sum of k squares).  Uses the theorems on sums of 1, 2, 3 and 4 squares. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* If this holds, n can be expressed as a sum of two squares. */
static inline bool can_sum_two(idx_t n)
{
    if (n == 0) return true;          /* zero is trivially a sum of two squares */

    /* Remove factor 2, it never affects the 3 mod 4 condition */
    while ((n & 1) == 0) n >>= 1;

    /* Factors of n that might be 3 mod 4 are odd primes */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            /* Only odd exponents of primes ≡ 3 (mod 4) disqualify n */
            if ((p & 3) == 3 && (exp & 1))
                return false;
        }
    }

    /* If a prime > sqrt(original n) remains, it is prime itself */
    if (n > 1 && (n & 3) == 3)
        return false;

    return true;
}

/* Return true iff all primes p ≡ 3 (mod 4) divide n with even exponent. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, but guard */

    /* 1 square (k=1): n must be a perfect square */
    seq_t s = floor_sqrt((seq_t)n);
    if (s * s == (seq_t)n)
        return (seq_t)1;

    /* 2 squares (k=2): n must have no prime ≡3 (mod 4) with odd exponent */
    if (can_sum_two(n))
        return (seq_t)2;

    /* 3 squares (k=3): n*3 must NOT be of the form 4^a(8b+7) */
    seq_t m = (seq_t)n * 3;
    while (m % 4 == 0)
        m /= 4;
    if (m % 8 == 7)        /* 3n is of forbidden form → need 4 squares */
        return (seq_t)4;

    return (seq_t)3;       /* otherwise 3 squares suffice */
}
