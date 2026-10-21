/*
 * A295440  a(n) = (18*n)!*(4*n)!*(3*n)!/((9*n)!*(8*n)!*(6*n)!*(2*n)!).
 * https://oeis.org/A295440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n)!*(4*n)!*(3*n)!/((9*n)!*(8*n)!*(6*n)!*(2*n)!) =
 *            binomial(18*n,9*n)*binomial(9*n,n)*binomial(3*n,n)/
 *            (binomial(6*n,2*n)*binomial(2*n,n))
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Binomial coefficients */
    seq_t A = binomial((idx_t)(18 * n), (idx_t)(9 * n));
    seq_t B = binomial((idx_t)(9 * n), n);
    seq_t C = binomial((idx_t)(3 * n), n);
    seq_t E = binomial((idx_t)(6 * n), (idx_t)(2 * n));
    seq_t F = binomial((idx_t)(2 * n), n);

    /* Arrays of numerators and denominators */
    seq_t num[3] = { A, B, C };
    seq_t den[2] = { E, F };

    /* Cancel common factors between numerators and denominators
       iteratively until all denominators are 1. */
    while (1) {
        int i, j;
        int changed = 0;
        for (i = 0; i < 3; ++i) {
            for (j = 0; j < 2; ++j) {
                if (num[i] == (seq_t)1 || den[j] == (seq_t)1)
                    continue;
                seq_t g = gcd(num[i], den[j]);
                if (g != (seq_t)1) {
                    num[i] /= g;
                    den[j] /= g;
                    changed = 1;
                }
            }
        }
        if (!changed)
            break;
    }

    /* All denominators should now be 1.  Multiply numerators */
    seq_t result = (seq_t)1;
    result *= num[0];
    result *= num[1];
    result *= num[2];

    return result;
}
