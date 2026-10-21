/*
 * A261884  Expansion of (a(q) - a(q^2) - 2*a(q^3) + 2*a(q^6)) / 6 in powers
 *          of q where a() is a cubic AGM function.
 * https://oeis.org/A261884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   prime_contribution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A261884
 *  a(n) is multiplicative with
 *      a(2^e)      = (-1)^e
 *      a(3^e)      = -1   (for any e ≥ 1)
 *      a(p^e)      = e + 1     if p ≡ 1 (mod 6)
 *      a(p^e)      = (1 + (-1)^e)/2  if p ≡ 5 (mod 6)
 *  This function factors n with trial division and
 *  multiplies the prime‑power contributions.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/prime_contribution.h"

seq_t a(idx_t n)
{
    if (n == 1ULL) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t t = (uint64_t)n;

    for (uint64_t p = 2; (unsigned __int128)p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            seq_t val = prime_contribution(p, e);
            result *= val;
            if (result == 0) return (seq_t)0;
        }
    }
    if (t > 1ULL) {            /* remaining prime factor */
        seq_t val = prime_contribution(t, 1);
        result *= val;
    }
    return result;
}
