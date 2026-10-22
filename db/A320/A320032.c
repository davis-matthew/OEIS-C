/*
 * A320032  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of the e.g.f. exp(-x)/(1 - k*x).
 * https://oeis.org/A320032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   * a(n) = A(i,j) where the square array A(m,k) is read by *
 *            antidiagonals. Let d be the largest integer with * d*(d+1)/2 <=
 *            n. Then * i = n - d*(d+1)/2 // row index * j = d - i // column
 *            index * and * A(i,j) = Σ_{t=0..i} (-1)^(i-t) * binomial(i,t) *
 *            t! * j^t * This implements the OEIS definition o...
 * Range:     i8 n <= 24, i16 n <= 49, i32 n <= 86, i64 n <= 184,
 *            i128 n <= 428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- find the diagonal number d ------------------------------------ */
    uint64_t low = 0, high = (uint64_t)n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    uint64_t d = low;                           /* column index */
    unsigned __int128 base = (unsigned __int128)d * (d + 1) / 2;
    uint64_t i = n - (uint64_t)base;             /* row index   */
    uint64_t j = d - i;                          /* column index */

    /* --- compute A(i,j) ---------------------------------------------- */
    seq_t sum = 0;
    seq_t bin = 1;          /* binomial coefficient C(i,0) */
    seq_t fact = 1;         /* factorial 0!  */
    seq_t powk = 1;         /* j^0           */

    for (uint64_t t = 0; t <= i; ++t) {
        seq_t term = bin * fact * powk;
        if (((i - t) & 1) != 0)
            sum -= term;
        else
            sum += term;

        if (t == i) break;   /* last iteration – no update needed */

        /* update binomial coefficient C(i,t+1) from C(i,t) */
        seq_t numerator   = (seq_t)(i - t);
        seq_t denominator = (seq_t)(t + 1);
        bin = bin * numerator / denominator;

        /* update factorial                                 */
        fact *= (seq_t)(t + 1);

        /* update power of j                               */
        powk *= (seq_t)j;
    }

    return sum;
}
