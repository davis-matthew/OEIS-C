/*
 * A344401  a(n) = [x^n] 24*(-40*x^4 + 49*x^3 - 15*x^2 + 13*x + 2) / (1 -
 *          4*x)^(9/2).
 * https://oeis.org/A344401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=48, a(n)=6*(3*n+4)*(2*n^3+9*n^2+13*n+4)*binomial(2*n-1,n)
 *            for n>=1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 22, u128 n <= 52,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)48;

    seq_t N  = (seq_t)n;

    /* Compute n^2 and n^3 */
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* 2*n^3 + 9*n^2 + 13*n + 4 */
    seq_t poly = 2 * n3 + 9 * n2 + 13 * N + 4;

    /* 3*n + 4 */
    seq_t part = 3 * N + 4;

    /* 6 * (3*n+4) * poly */
    seq_t temp = 6 * part * poly;

    /* binomial(2*n-1, n) */
    seq_t binom = binomial((n << 1) - 1, n);

    return temp * binom;
}
