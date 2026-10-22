/*
 * A302690  a(n) is the smallest integer m such that m*n is a sum of two
 *          squares but not one.
 * https://oeis.org/A302690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that n*m is a sum of two squares but not
 *            a perfect square
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the minimal m for a given n. */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    seq_t t = 1;                 /* product of 3 mod 4 primes with odd exponent */
    bool has3odd   = false;      /* any p ≡ 3 (mod 4) with odd exponent? */
    bool hasOthOdd = false;      /* any other prime with odd exponent? */

    uint64_t temp = nn;
    /* factor 2 */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e & 1)            /* odd exponent of 2 -> counts as 'other odd' */
        hasOthOdd = true;

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if ((p & 3ULL) == 3ULL) {          /* p ≡ 3 (mod 4) */
                if (e & 1) {
                    has3odd = true;
                    t *= (seq_t)p;
                }
            } else {                           /* p ≡ 1 (mod 4) or 2 */
                if (e & 1)
                    hasOthOdd = true;
            }
        }
    }

    /* remaining prime factor > sqrt(original temp) */
    if (temp > 1) {
        if ((temp & 3ULL) == 3ULL) {          /* remaining prime ≡ 3 (mod 4) */
            has3odd = true;
            t *= (seq_t)temp;
        } else {                               /* remaining prime ≡ 1 (mod 4) or 2 */
            hasOthOdd = true;
        }
    }

    /* Determine the answer according to the rules described in the analysis. */
    if (!has3odd && !hasOthOdd) {
        /* n is a perfect square; minimal m is 2. */
        return (seq_t)2;
    }

    if (!has3odd && hasOthOdd) {
        /* n is not a square but already sum‑of‑two‑squares; minimal m is 1. */
        return (seq_t)1;
    }

    if (!hasOthOdd) {
        /* n* t is a square → multiply by 2 as well. */
        return (seq_t)2 * t;
    }

    /* n* t is not a square → minimal m is t. */
    return t;
}
