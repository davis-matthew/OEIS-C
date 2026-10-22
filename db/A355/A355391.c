/*
 * A355391  Position of first appearance of n in A181591 =
 *          binomial(bigomega(n), omega(n)).
 * https://oeis.org/A355391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of first appearance of n in A181591 * i.e. a(n)
 *            = min{ m > 0 : binomial(bigomega(m), omega(m)) = n } * Above,
 *            binomial is the standard definition of the binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 30, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static const uint64_t small_primes[64] = {
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,
    233,239,241,251,257,263,269,271,277,281,
    283,293,307,311
};

seq_t a(idx_t n)
{
    if (n == 1) /* a(1) = 1 by definition */
        return (seq_t)1;

    /* Candidate with w = 1 (b = n):  m = 2^n */
    seq_t best = pow2(n);

    /* Search for other pairs (b, w) with w ≥ 2 that produce n */
    const idx_t wLimit = 64;          /* sufficient for n ≤ 10000 */
    for (idx_t w = 2; w <= wLimit; ++w) {
        /* b must be at least w */
        for (idx_t b = w; ; ++b) {
            seq_t val = binomial(b, w);
            if (val > n)
                break;

            if (val == n) {
                /* minimal m for (b, w):
                 * m = 2^(b-(w-1)) × product of primes[1 .. w-1]
                 */
                seq_t m = pow2(b - (w - 1));
                for (idx_t i = 1; i < w; ++i)
                    m *= small_primes[i];

                if (best == 0 || m < best)
                    best = m;
                /* For a fixed w, val is strictly increasing in b,
                 * so no other b will match n for this w. */
                break;
            }
        }
    }

    return best;
}
