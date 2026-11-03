/*
 * A336577  a(n) = Sum_{k=0..n} 2^k * binomial(n,k) *
 *          binomial(n^2+k+1,n)/(n^2+k+1).
 * https://oeis.org/A336577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 2^k * binomial(n,k) *
 *            binomial(n^2+k+1,n)/(n^2+k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 285 (2026-11-01)
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses GCD reductions to avoid overflow; works for any seq_t type. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t n_sq = n * n;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Denominator N = n^2 + k + 1 */
        idx_t N = n_sq + k + 1;
        seq_t denom = (seq_t)N;          /* current denominator for this term */
        seq_t product = (seq_t)1;        /* product of factors after reductions */

        /* ----- factor 1: binomial(n,k) ----- */
        seq_t factor = binomial(n, k);
        seq_t g = gcd(factor, denom);
        if (g != (seq_t)1) {
            factor /= g;
            denom /= g;
        }
        product *= factor;
        if (denom != (seq_t)1) {
            g = gcd(product, denom);
            if (g != (seq_t)1) {
                product /= g;
                denom /= g;
            }
        }

        /* ----- factor 2: binomial(n^2 + k + 1, n) ----- */
        factor = binomial(n_sq + k + 1, n);
        g = gcd(factor, denom);
        if (g != (seq_t)1) {
            factor /= g;
            denom /= g;
        }
        product *= factor;
        if (denom != (seq_t)1) {
            g = gcd(product, denom);
            if (g != (seq_t)1) {
                product /= g;
                denom /= g;
            }
        }

        /* ----- factor 3: 2^k ----- */
        factor = pow2(k);
        g = gcd(factor, denom);
        if (g != (seq_t)1) {
            factor /= g;
            denom /= g;
        }
        product *= factor;
        if (denom != (seq_t)1) {
            g = gcd(product, denom);
            if (g != (seq_t)1) {
                product /= g;
                denom /= g;
            }
        }

        /* Final division by any remaining denominator part */
        if (denom != (seq_t)1) {
            product /= denom;
        }

        sum += product;
    }

    return sum;
}
