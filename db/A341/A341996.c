/*
 * A341996  a(n) = 1 if there is at least one such prime p that p^p divides
 *          the arithmetic derivative of n, A003415(n); a(0) = a(1) = 0 by
 *          convention.
 * https://oeis.org/A341996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ prime p with p^p | A003415(n) (arithmetic
 *            derivative of n), else 0. For n=0,1 the function returns 0 by
 *            convention
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9555 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool factor_and_check(seq_t derivative)
{
    /* Return true if some prime p satisfies p^p | derivative. */
    seq_t rem = derivative;
    idx_t p = 2;

    /* Handle the case when derivative is 0 or 1 */
    if (rem == 0 || rem == 1)
        return false;

    /* Factorization loop */
    while ((seq_t)p * (seq_t)p <= rem) {
        int exp = 0;
        while (rem % p == 0) {
            exp++;
            rem /= p;
        }
        if (exp >= (int)p)          /* p is a prime, exponent large enough */
            return true;
        p++;
    }

    /* If a prime factor larger than sqrt remains */
    if (rem > 1) {   /* rem is prime */
        /* For a prime rem, exponent is 1, so 1 >= rem never holds for p ≥ 2. */
        /* No need to check further. */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Special cases */
    if (n <= 1)
        return 0;

    /* Factor n and compute its arithmetic derivative */
    seq_t derivative = 0;
    idx_t nn = n;
    idx_t p = 2;
    while (p * p <= nn) {
        if (nn % p == 0) {
            int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            /* term = exp * (n / p) */
            idx_t term = (n / p) * exp;
            derivative += (seq_t)term;
        }
        p++;
    }
    if (nn > 1) { /* remaining prime factor */
        /* term = 1 * (n / nn) */
        idx_t term = n / nn;   /* nn is a prime > 1 */
        derivative += (seq_t)term;
    }

    /* Check whether any prime p satisfies p^p | derivative */
    return factor_and_check(derivative) ? 1 : 0;
}
