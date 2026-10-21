/*
 * A295448  a(n) = (18*n)!*(3*n)!*(2*n)!/((9*n)!*(7*n)!*(6*n)!*n!).
 * https://oeis.org/A295448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(18*n,9*n)*binomial(9*n,2*n)*binomial(2*n,n) /*
 *            /(binomial(6*n,3*n)*binomial(3*n,n))
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* numerator factors */
    seq_t num[3];
    /* denominator factors */
    seq_t den[2];

    num[0] = binomial((idx_t)18*n, (idx_t)9*n);
    num[1] = binomial((idx_t)9*n,  (idx_t)2*n);
    num[2] = binomial((idx_t)2*n,   (idx_t)n);

    den[0] = binomial((idx_t)6*n,   (idx_t)3*n);
    den[1] = binomial((idx_t)3*n,   (idx_t)n);

    /* cancel denominator factors with numerators */
    for (size_t di = 0; di < 2; ++di) {
        while (den[di] > 1) {
            for (size_t ni = 0; ni < 3; ++ni) {
                seq_t g = gcd(num[ni], den[di]);
                if (g > 1) {
                    num[ni] /= g;
                    den[di] /= g;
                    if (den[di] == 1)
                        break;
                }
            }
        }
    }

    /* multiply the remaining numerators */
    seq_t res = 1;
    for (size_t i = 0; i < 3; ++i) {
        res *= num[i];
    }

    return res;
}
