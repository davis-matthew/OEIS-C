/*
 * A399248  Rectangular array read by descending antidiagonals: r(n, k) =
 *          floor(k*x^n), where x = 1+sqrt(3).
 * https://oeis.org/A399248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = floor( k * (1+sqrt(3))^row ) where row and k are obtained
 *            from the antidiagonal ordering of the array r(n,k) = floor( k *
 *            (1+sqrt(3))^n )
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Locate the diagonal (s = row + k) and the position t on it. */
    idx_t s = 2;                /* diagonal sum s (starting from 2)      */
    idx_t cumulative = 1;       /* number of terms up to and including this diagonal */
    while (cumulative < n) {
        ++s;
        cumulative += s - 1;    /* add the number of terms on this diagonal */
    }
    idx_t prev = cumulative - (s - 1);   /* terms before this diagonal */
    idx_t t = n - prev;          /* 1‑based row index on this diagonal      */
    idx_t k = s - t;             /* column index (also 1‑based)              */

    /* Compute the integers A_row and B_row such that
       (1+sqrt(3))^row = A_row + B_row*sqrt(3). */
    seq_t A = 1;    /* A_1 */
    seq_t B = 1;    /* B_1 */
    if (t > 1) {
        for (idx_t i = 2; i <= t; ++i) {
            seq_t newA = A + 3 * B;  /* A_{i} = A_{i-1} + 3*B_{i-1}   */
            seq_t newB = A + B;      /* B_{i} = A_{i-1} + B_{i-1}     */
            A = newA;
            B = newB;
        }
    }

    /* Floor( k * (A + B*sqrt(3)) ) = k*A + floor( k*B*sqrt(3) ). */
    seq_t sk = (seq_t)k;
    seq_t part1 = seq_mul(sk, A);      /* k * A */
    seq_t m     = seq_mul(sk, B);      /* m = k * B */

    /* floor(m * sqrt(3)) = floor( sqrt( 3 * m^2 ) ). */
    seq_t msq = seq_mul(m, m);         /* m^2 */
    seq_t prod = seq_mul(msq, (seq_t)3);/* 3 * m^2 */
    seq_t part2 = floor_sqrt(prod);     /* floor( sqrt(3 * m^2) ) */

    return part1 + part2;
}
