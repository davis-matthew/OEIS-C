/*
 * A345340  The number of squares with vertices from the vertices of the
 *          n-dimensional hypercube.
 * https://oeis.org/A345340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) * Sum_{k=1..floor(n/2)} n!/(2*k!*k!*(n-2*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 0 */
    if (n < 2)
        return 0;

    seq_t sum = 0;
    idx_t maxk = n / 2;

    seq_t bn2k = 0;   /* binom(n, 2k)  */
    seq_t bnk   = 0;   /* binom(2k, k)  */

    for (idx_t k = 1; k <= maxk; ++k) {
        if (k == 1) {
            /* binom(n,2) = n*(n-1)/2   */
            bn2k = (seq_t)n * (n - 1) / 2;
            /* binom(2,1) = 2            */
            bnk = 2;
        } else {
            /* Update binom(n, 2k) from binom(n, 2(k-1)) */
            idx_t r = 2 * k;  /* current r = 2k */
            seq_t num1 = (seq_t)(n - r + 1);  /* n-2k+1 */
            seq_t num2 = (seq_t)(n - r + 2);  /* n-2k+2 */
            seq_t den1 = (seq_t)r;            /* 2k   */
            seq_t den2 = (seq_t)(r - 1);      /* 2k-1 */
            bn2k = bn2k * num1;
            bn2k = bn2k * num2;
            bn2k = bn2k / den1;
            bn2k = bn2k / den2;

            /* Update binom(2k, k) from binom(2(k-1), k-1) */
            seq_t num3 = (seq_t)(2 * k - 1);   /* 2k-1 */
            seq_t num4 = (seq_t)(2 * k);       /* 2k   */
            seq_t den3 = (seq_t)k;             /* k    */
            seq_t den4 = (seq_t)k;             /* k    */
            bnk = bnk * num3;
            bnk = bnk * num4;
            bnk = bnk / den3;
            bnk = bnk / den4;
        }

        seq_t term = bn2k * bnk;
        term = term / 2;     /* exact division by 2 */
        sum += term;
    }

    /* factor = 2^(n-2) */
    seq_t factor = 1;
    factor <<= (n - 2);

    return sum * factor;
}
