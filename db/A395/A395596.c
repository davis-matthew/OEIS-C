/*
 * A395596  Nearest integer to n*(phi-1) which is coprime with n, where phi is
 *          the golden ratio.
 * https://oeis.org/A395596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*(phi-1) that is coprime to n
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9865 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
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

seq_t a(idx_t n)
{
    if (n <= (idx_t)0) return (seq_t)0;   /* undefined for n=0, safe fallback */

    /* Compute floor(sqrt(5*n^2)) */
    seq_t prod = (seq_t)5 * (seq_t)n * (seq_t)n;
    seq_t sqrt5n = floor_sqrt(prod);

    /* theta = floor(n*sqrt(5)) - n */
    seq_t theta = sqrt5n - (seq_t)n;

    /* m0 = nearest integer to theta/2 (ceil for odd theta) */
    seq_t m0 = (theta + (theta & (seq_t)1)) / (seq_t)2;

    /* If m0 already coprime, return it */
    if (gcd(m0, (seq_t)n) == (seq_t)1)
        return m0;

    /* Decide search direction: if theta odd, m0 > true value → check lower first */
    bool prefer_low = ((theta & (seq_t)1) != 0);

    /* Search adjacent integers until one is coprime with n */
    for (idx_t d = 1; ; ++d) {
        if (prefer_low) {
            /* Lower neighbour first */
            if (m0 > (seq_t)d) {
                seq_t cand = m0 - (seq_t)d;
                if (gcd(cand, (seq_t)n) == (seq_t)1)
                    return cand;
            }
            /* Then higher neighbour */
            seq_t cand = m0 + (seq_t)d;
            if (gcd(cand, (seq_t)n) == (seq_t)1)
                return cand;
        } else {
            /* Higher neighbour first */
            seq_t cand = m0 + (seq_t)d;
            if (gcd(cand, (seq_t)n) == (seq_t)1)
                return cand;
            /* Then lower neighbour */
            if (m0 > (seq_t)d) {
                cand = m0 - (seq_t)d;
                if (gcd(cand, (seq_t)n) == (seq_t)1)
                    return cand;
            }
        }
    }
}
