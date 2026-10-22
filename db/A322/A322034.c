/*
 * A322034  Let p1 <= p2 <= ... <= pk be the prime factors of n, with
 *          repetition; let s = 1/p1 + 1/(p1*p2) + 1/(p1*p2*p3) + ... +
 *          1/(p1*p2*...*pk); a(n) = numerator of s. a(1)=0 by convention.
 * https://oeis.org/A322034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum s = ∑_{i=1..k} 1/(p₁⋯pᵢ) where
 *            p₁≤p₂≤⋯≤p_k are the prime factors of n with multiplicity. a(1) =
 *            0. The computation factors n by trial division, accumulates the
 *            terms n/(p₁⋯pᵢ), then divides by gcd with n
 * Range:     u8 n <= 286, u16 n <= 9764, u32 n <= 9764, u64 n <= 9764,
 *            u128 n <= 9764, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    /* original value of n – used for the final division */
    const uint64_t orig = (uint64_t)n;
    /* sum of the terms n/(p₁⋯pᵢ) (will be reduced at the end) */
    seq_t sum = (seq_t)0;
    /* running product of the prime factors processed so far */
    uint64_t pref = 1;
    uint64_t m = orig;

    /* factor 2 repeatedly */
    while ((m & 1) == 0) {
        pref *= 2ULL;
        sum += (seq_t)(orig / pref);
        m >>= 1;
    }
    /* factor odd primes */
    for (uint64_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) {
            pref *= i;
            sum += (seq_t)(orig / pref);
            m /= i;
        }
    }
    /* if remaining m > 1 it is a prime factor */
    if (m > 1) {
        pref *= m;
        sum += (seq_t)(orig / pref);
    }

    seq_t g = gcd(sum, (seq_t)orig);
    return sum / g;
}
