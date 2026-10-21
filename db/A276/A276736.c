/*
 * A276736  a(n) = numerator of Sum_{d|n} tau(d)/d.
 * https://oeis.org/A276736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} tau(d)/d
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Preserve the original value for the final gcd. */
    idx_t n_original = n;
    /* Temporary variable for factorisation. */
    idx_t temp = n;

    /* Running product of the numerator parts A = ∏_p num(p^e). */
    seq_t prod_A = 1;

    /* Trial division up to sqrt(temp). */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* Count exponent e of prime p in n. */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Compute p^e as seq_t. */
            seq_t pow_e = 1;
            for (idx_t i = 0; i < e; ++i)
                pow_e *= (seq_t)p;

            /* Sum_{k=0..e} (k+1) * p^{e-k} */
            seq_t cur = pow_e;
            seq_t num_p = 0;
            for (idx_t k = 0; k <= e; ++k) {
                num_p += (seq_t)(k + 1) * cur;
                cur /= (seq_t)p;   /* integer division, remainder is zero */
            }

            prod_A *= num_p;
        }
    }

    /* If n has a remaining prime factor > sqrt(original n). */
    if (temp > 1) {
        /* Remaining prime has exponent e = 1. */
        seq_t pow_e = (seq_t)temp;   /* p^1 */
        seq_t cur = pow_e;
        seq_t num_p = 0;
        /* k = 0 : (1)*p^1 */
        num_p += (seq_t)1 * cur;
        cur /= (seq_t)temp;          /* becomes 1 */
        /* k = 1 : (2)*p^0 */
        num_p += (seq_t)2 * cur;
        prod_A *= num_p;
    }

    /* The sum S = Sum_{d|n} tau(d)/d equals prod_A / n_original.
       Reduce that fraction and return its numerator. */
    seq_t g = gcd(prod_A, (seq_t)n_original);
    return prod_A / g;
}
