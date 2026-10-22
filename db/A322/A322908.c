/*
 * A322908  The determinant of an n X n Toeplitz matrix M(n) whose first row
 *          consists of successive positive integer numbers 1, ..., n and
 *          whose first column consists of 1, n + 1, ..., 2*n - 1.
 * https://oeis.org/A322908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1)*((3*n + 1)*(n + 1)^(n-1) - (n - 1)^n)/4
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = -5, a(3) = 38, ... */
    if (n == 0) return 0;            /* not used, but safe for completeness */

    seq_t N = (seq_t)n;               /* convert n to the working type */

    /* (n + 1)^(n-1)  and  (n - 1)^n  */
    seq_t term1 = big_pow(N + 1, (seq_t)(n - 1));
    seq_t term2 = big_pow(N - 1, N);

    /* (3*n + 1)*(n + 1)^(n-1)  */
    seq_t coeff = 3 * N + 1;
    seq_t prod  = term1 * coeff;

    /* difference and division by 4 */
    seq_t diff = prod - term2;
    seq_t res  = diff / 4;

    /* sign factor: (-1)^(n-1) */
    if ((n & 1) == 0)                /* n even  => negative sign */
        res = -res;

    return res;
}
