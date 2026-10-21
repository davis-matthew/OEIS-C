/*
 * A295302  a(n) = sigma(phi(n)) - n, where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(phi(n)) - n
 * Range:     i8 n <= 96, i16 n <= 9809, i32 n <= 9809, i64 n <= 9809,
 *            i128 n <= 9809, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A295302: a(n) = sum_of_divisors(phi(n)) - n. */

#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phival = phi(n);

    /* φ(n) ≤ n, so it fits in idx_t for all n that can be indexed. */
    idx_t phidx = (idx_t)phival;

    /* Compute σ(φ(n)) */
    seq_t sigma_val = sigma_of(phidx);

    /* Return σ(φ(n)) - n */
    return sigma_val - (seq_t)n;
}
