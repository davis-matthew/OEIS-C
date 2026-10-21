/*
 * A285046  Expansion of cosh(9*arctanh(2*sqrt(x))).
 * https://oeis.org/A285046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/105)*(4096*n^4 + 512*n^3 + 3392*n^2 + 400*n +
 *            105)*binomial(2*n,n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;

    /* polynomial part */
    seq_t poly = (seq_t)4096 * n4
               + (seq_t)512   * n3
               + (seq_t)3392  * n2
               + (seq_t)400   * sn
               + (seq_t)105;

    /* binomial part */
    seq_t bin = binomial(n + n, n);

    /* cancel the denominator 105 = 3*5*7 between poly and bin */
    const int factors[3] = {3, 5, 7};
    seq_t den = (seq_t)105;

    for (int i = 0; i < 3 && den > 1; ++i) {
        int p = factors[i];
        if (den % p == 0) {
            if (bin % p == 0) {
                bin /= (seq_t)p;
                den /= (seq_t)p;
            } else if (poly % p == 0) {
                poly /= (seq_t)p;
                den /= (seq_t)p;
            }
        }
    }

    /* final multiplication and division */
    seq_t result = seq_mul(poly, bin);
    if (den > 1) result /= den;

    return result;
}
