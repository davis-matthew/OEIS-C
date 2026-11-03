/*
 * A256750  Start with n, and repeatedly apply the arithmetic derivative
 *          A003415. |a(n)| = the number of iterations to reach 0 (then a(n)
 *          is taken nonnegative) or a number having a factor of the form p^p
 *          with prime p, in which case a(n) = -|a(n)|.
 * https://oeis.org/A256750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Arithmetic derivative iterations until reaching 0 or a number
 *            with a prime * factor p whose exponent e is ≥ p. The value is
 *            the number of steps if the * termination is 0, otherwise the
 *            negative of the number of steps to reach the * first qualifying
 *            number. a(0)=0, a(1)=1. The recurrence follows d...
 * Range:     i8 n <= 845, i16 n <= 65536, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 8208 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a 64‑bit integer has a prime factor p with exponent e ≥ p. */
static bool has_pp_factor_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        unsigned int e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        if (e >= 2)   /* p = 2 */
            return true;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            unsigned int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            if (e >= p) /* p itself is the exponent threshold */
                return true;
        }
    }

    /* remaining prime factor */
    if (n > 1) {
        /* exponent = 1, cannot satisfy e ≥ p for prime p ≥ 2 */
    }

    return false;
}

/* Compute the arithmetic derivative of a 64‑bit integer n. */
static seq_t derivative_uint64(uint64_t n)
{
    seq_t result = 0;
    uint64_t rem = n;

    /* factor 2 */
    if ((rem & 1ULL) == 0) {
        unsigned int e = 0;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
            ++e;
        }
        result += (seq_t)e * ((seq_t)n / 2);
    }

    /* odd factors */
    for (uint64_t p = 3; p <= rem / p; p += 2) {
        if (rem % p == 0) {
            unsigned int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            result += (seq_t)e * ((seq_t)n / p);
        }
    }

    /* remaining prime factor */
    if (rem > 1) {
        /* exponent = 1 */
        result += (seq_t)1 * ((seq_t)n / rem);
    }

    return result;
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) = 0 */
    if (n == 1)
        return 1;                 /* a(1) = 1 */

    /* Work with 64-bit values – the sequence stays within 64 bits for the
     * range of indices used in the tests, so the cast is safe. */
    uint64_t cur = (uint64_t)n;

    if (has_pp_factor_uint64(cur))
        return 0;                 /* already a fixed point */

    idx_t steps = 0;
    while (true) {
        seq_t d = derivative_uint64(cur);
        ++steps;

        if (d == 0)
            return (seq_t)steps;   /* reached zero */

        uint64_t d64 = (uint64_t)d;
        if (has_pp_factor_uint64(d64))
            return -(seq_t)steps;  /* reached p^p‑type fixed point */

        cur = d64;
    }
}
