/*
 * A349497  a(n) is the smallest element in the continued fraction of the
 *          harmonic mean of the divisors of n.
 * https://oeis.org/A349497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest element in the continued fraction of the
 *            harmonic mean of the divisors of n. The harmonic mean equals n *
 *            τ(n) / σ(n), where τ is the divisor count and σ is the sum of
 *            divisors. The continued fraction of a rational number a/b is
 *            obtained by the Euclidean algorithm and its partial...
 * Range:     u8 n <= 35, u16 n <= 6147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/tau_sigma.h"

seq_t a(idx_t n)
{
    seq_t tau, sigma;
    tau_sigma(n, &tau, &sigma);

    seq_t num = (seq_t)n * tau;   /* n * τ(n)   */
    seq_t den = sigma;            /* σ(n)       */

    seq_t min_q = 0;
    bool first = true;

    /* Euclidean algorithm to compute continued fraction quotients
       and keep the smallest one. */
    while (den != 0) {
        seq_t q = num / den;
        if (first || q < min_q) {
            min_q = q;
            first = false;
        }
        seq_t rem = num % den;
        num = den;
        den = rem;
    }

    return min_q;
}
