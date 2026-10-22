/*
 * A379223  Sum of the divisors of the n-th odd square: a(n) =
 *          sigma((2*n-1)^2).
 * https://oeis.org/A379223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma((2*n-1)^2)
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 8412, u64 n <= 8412,
 *            u128 n <= 8412, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Computes the sum of the divisors of the n-th odd square.
 * a(n) = σ((2*n-1)^2), where σ(m) is the sum of all positive divisors of m.
 */
seq_t a(idx_t n)
{
    /* t = 2*n - 1 (odd) */
    idx_t t = 2 * n - 1;
    seq_t result = (seq_t)1;

    /* Trial‑division of t.  Only odd divisors are checked. */
    for (idx_t i = 3; i <= t / i; i += 2) {
        if (t % i == 0) {
            /* Count exponent e of prime i in t */
            idx_t e = 0;
            while (t % i == 0) {
                ++e;
                t /= i;
            }
            /* For t² the exponent becomes 2*e.  The sigma factor is
             * (p^(2*e+1)-1)/(p-1) where p=i. */
            seq_t p = (seq_t)i;
            idx_t exp = 2 * e + 1;              /* exponent for the power */
            seq_t pow = (seq_t)1;
            for (idx_t k = 0; k < exp; ++k)
                pow *= p;
            seq_t numerator   = pow - (seq_t)1;
            seq_t denominator = p - (seq_t)1;
            seq_t term = numerator / denominator;
            result *= term;
        }
    }

    /* Remaining prime factor, if any, has exponent 1 in t → 2 in t². */
    if (t > 1) {
        seq_t p = (seq_t)t;
        seq_t term = (seq_t)1 + p + p * p;    /* (p³-1)/(p-1) = 1+p+p² */
        result *= term;
    }

    return result;
}
