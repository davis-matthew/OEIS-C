/*
 * A295302  a(n) = sigma(phi(n)) - n, where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(phi(n)) - n
 * Range:     i8 n <= 96, i16 n <= 12600, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 98 terms through n = 9809 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A295302: a(n) = sum_of_divisors(phi(n)) - n. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phival = totient(n);

    /* φ(n) ≤ n, so it fits in idx_t for all n that can be indexed. */
    idx_t phidx = (idx_t)phival;

    /* Compute σ(φ(n)) */
    seq_t sigma_val = sigma_of(phidx);

    /* Return σ(φ(n)) - n */
    return sigma_val - (seq_t)n;
}
