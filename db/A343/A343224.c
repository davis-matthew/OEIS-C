/*
 * A343224  a(n) = sigma(n) - A003415(n), where A003415 is the arithmetic
 *          derivative, and sigma is the sum of divisors of n.
 * https://oeis.org/A343224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - A003415(n) (arithmetic derivative)
 * Range:     i8 n <= 109, i16 n <= 8191, i32 n <= 8238, i64 n <= 8238,
 *            i128 n <= 8238, big any n
 * Helpers:   deriv_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/deriv_u64.h"

/* Compute the sum of divisors of a uint64_t value n. */
static seq_t sigma_u64(uint64_t n)
{
    seq_t sigma = (seq_t)1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = (seq_t)1;
            seq_t sum  = (seq_t)1;
            while (m % p == 0) {
                m /= p;
                term *= (seq_t)p;
                sum  += term;
            }
            sigma *= sum;
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)1 + (seq_t)m);
    }
    return sigma;
}

/* OEIS sequence A343224: a(n) = sigma(n) - A003415(n) */
seq_t a(idx_t n)
{
    /* n is a positive integer (offset 1), use 1-based indexing */
    uint64_t val = (uint64_t)n;
    seq_t sigma = sigma_u64(val);
    seq_t deriv = deriv_u64(val);
    return sigma - deriv;
}
