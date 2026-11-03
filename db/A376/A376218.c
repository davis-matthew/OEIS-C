/*
 * A376218  Odd exponentially odd numbers.
 * https://oeis.org/A376218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9925 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth odd integer whose prime factorisation
 * contains only odd primes and only odd exponents.
 * We enumerate odd integers, test each for the property,
 * and stop when we have found n such numbers. */

#include <stdbool.h>

/* Test whether a seq_t satisfies the odd‑exponent condition. */
static bool good_seq(seq_t x)
{
    if (x == (seq_t)0)             /* nonsense, but avoid */
        return false;
    if ((x % (seq_t)2) == (seq_t)0)/* 2 is forbidden */
        return false;

    seq_t val = x;
    for (seq_t p = (seq_t)3; p * p <= val; p += (seq_t)2) {
        if (val % p == (seq_t)0) {
            unsigned exp = 0;
            do {
                val /= p;
                ++exp;
            } while (val % p == (seq_t)0);
            if ((exp & 1) == 0)    /* even exponent not allowed */
                return false;
        }
    }
    /* Remaining val is 1 or an odd prime with exponent 1 (odd). */
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset safety; not used */
        return (seq_t)0;

    idx_t  count  = 0;
    seq_t  candidate = (seq_t)1;
    seq_t  result   = (seq_t)0;

    while (count < n) {
        if (good_seq(candidate)) {
            ++count;
            result = candidate;
        }
        ++candidate;
    }
    return result;
}
