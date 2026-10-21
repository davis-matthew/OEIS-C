/*
 * A249598  Analog of A097717 in base 4.
 * https://oeis.org/A249598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least positive integer N such that * rotating the
 *            base‑4 digits of N to the left yields M with * N = n * M. * The
 *            algorithm searches for the minimal digit length k, * computes
 *            the leading digit d (1 ≤ d < 4), and solves * N = d*4^(k-1) +
 *            rest where rest satisfies * rest = d*(4^(k-1) –...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The following types are defined by the build system:
 *   idx_t – unsigned integer type for indices (e.g. uint64_t)
 *   seq_t – unsigned integer type for sequence values
 */
seq_t a(idx_t n)
{
    const idx_t base = 4;
    seq_t nSeq  = (seq_t)n;
    /* Denominator used in the closed form: d * (pow - n) / (n*base - 1) */
    seq_t denom = (seq_t)n * (seq_t)base - (seq_t)1;

    seq_t pow = (seq_t)1;           /* pow = base^(k-1) for current k   */
    /* k = 1 corresponds to pow = 1. */
    for (idx_t k = 1; ; ++k) {
        /* If pow equals n, we have a trivial solution: N = n. */
        if (pow == nSeq) {
            /* Any leading digit d=1 gives N = d*pow = pow = n.
             * Since pow == n, this is the smallest possible N. */
            return nSeq;
        }
        /* A solution can only exist when pow > n, so skip earlier k. */
        if (pow > nSeq) {
            for (seq_t d = 1; d < base; ++d) {
                seq_t numerator = d * (pow - nSeq);   /* numerator ≥ 0 */
                if (numerator % denom == 0) {
                    seq_t rest = numerator / denom;   /* rest ≥ 0          */
                    if (rest < pow) {
                        /* Construct N = d * pow + rest. */
                        return d * pow + rest;
                    }
                }
            }
        }
        /* Prepare next power of base for the next k.  The loop stops
         * as soon as a solution is found, which always happens for
         * base = 4 and any integer n ≥ 1. */
        pow *= (seq_t)base;
    }
}
