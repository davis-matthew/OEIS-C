/*
 * A319448  Moebius function mu(n) defined for the Eisenstein integers.
 * https://oeis.org/A319448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 0 if 3 | n or n has a squared prime factor, otherwise
 *            a(n) = ∏_{p | n} (3 - 2*(p mod 3)). For n square‑free and n not
 *            divisible by 3 this product is ±1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9727 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute the Eisenstein Möbius value for a 64‑bit integer. */
static seq_t eig_mobius(uint64_t n)
{
    /* 0 for multiples of 3 because 3 = w'*(1+w)^2 contains a square factor. */
    if ((n % 3ULL) == 0ULL)
        return (seq_t)0;

    seq_t sign = 1;            /* sign accumulator */
    uint64_t x = n;            /* work copy */

    /* Trial division up to sqrt(x).  The loop end condition uses multiplication,
       which is safe for 64‑bit integers because the product is at most x*x ≤ 2^128
       which fits in a 128‑bit intermediate in C, but the loop guards are
       constructed to avoid overflow for 64‑bit inputs. */
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
                if (exp > 1)           /* not square‑free */
                    return (seq_t)0;
            }
            /* multiply by (3-2*(p mod 3)), which is 1 if p≡1 (mod3) or -1 if p≡2. */
            if ((p % 3ULL) == 2ULL)
                sign = -sign;
        }
    }

    /* Remaining factor x>1 is a prime (> sqrt(original n)). */
    if (x > 1) {
        if ((x % 3ULL) == 2ULL)
            sign = -sign;
    }

    return sign;
}

seq_t a(idx_t n)
{
    /* Cast idx_t to uint64_t for safety; n is always ≥1. */
    uint64_t m = (uint64_t)n;
    return eig_mobius(m);
}
