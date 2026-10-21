/*
 * A263765  Minimum number of squares necessary to write n as a sum or
 *          difference of squares.
 * https://oeis.org/A263765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Minimum number of squares (sum or difference) to represent n.
   The rule is:
     - a(n) = 1  if n is a perfect square,
     - a(n) = 2  if n is not a square and
                 either n ≡ 0,1,3 (mod 4)  (difference of two squares exists)
                 or n is a sum of two squares (Euler's theorem),
     - otherwise a(n) = 3. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n ≤ 2^64‑1 */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    /* Find the largest power of 4 ≤ n */
    idx_t bit = 1ULL << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* True iff n can be expressed as a sum of two squares.
   Uses the classic theorem: all primes ≡ 3 (mod 4) must occur with an even exponent. */
static bool sum_of_two_squares(idx_t n)
{
    if (n == 0) return true;               /* 0 = 0² + 0² */

    idx_t temp = n;
    /* Remove the factor 2 – it never affects the parity condition. */
    while ((temp & 1ULL) == 0) temp >>= 1;

    for (idx_t p = 3; ; p += 2) {
        if (p > temp / p) break;           /* p² > temp  →  stop */
        if (temp % p == 0) {
            int exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            if ((p & 3ULL) == 3ULL && (exp & 1) == 1)
                return false;              /* odd exponent of a 3‑mod‑4 prime */
        }
    }
    /* Remaining prime factor, if any */
    if (temp > 1 && (temp & 3ULL) == 3ULL)
        return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* 0 = 0² uses one square */

    /* Check for a perfect square */
    idx_t r = int_sqrt(n);
    if (r * r == n)
        return (seq_t)1;

    /* Numbers congruent to 2 mod 4 cannot be expressed as a difference
       of two squares.  For them we need to test the sum‑of‑two‑squares case. */
    if ((n & 3ULL) == 2ULL) {
        return sum_of_two_squares(n) ? (seq_t)2 : (seq_t)3;
    }

    /* All other n (≡0,1,3 (mod 4)) are differences of two squares. */
    return (seq_t)2;
}
