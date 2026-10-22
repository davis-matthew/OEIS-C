/*
 * A323967  Number of 3 X n integer matrices (m_{i,j}) such that m_{1,1}=0,
 *          m_{3,n}=2, and all rows, columns, and falling diagonals are
 *          (weakly) monotonic without jumps of 2.
 * https://oeis.org/A323967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + (((( ( ( (n+12)*n +55)*n +120)*n -236)*n -312)*n) /
 *            360 for n > 0, a(0) = 1 Uses the closed‐form a(n) = Σ_{j=0}^7
 *            q_j * C(n-j+6, 6) with q = { 1, -6, 18, -17, 3, 7, -5, 1 }.
 *            Computes binomial coefficients safely with 128‑bit intermediate
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 105, u64 n <= 4333,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute C(m,6) using 128–bit intermediate arithmetic */
static seq_t binom6(idx_t m)
{
    if (m < 6) return 0;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        res = res * (m - 6 + i);
        res /= i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    const int coeff[8] = { 1, -6, 18, -17, 3, 7, -5, 1 };

    unsigned __int128 acc = 0;
    for (int j = 0; j < 8; ++j) {
        idx_t m = n + 6 - j;
        seq_t b = binom6(m);

        unsigned __int128 abscoeff = (unsigned __int128)(coeff[j] >= 0 ? coeff[j] : -coeff[j]);
        unsigned __int128 prod = (unsigned __int128)b * abscoeff;

        if (coeff[j] >= 0)
            acc += prod;
        else
            acc -= prod;
    }

    return (seq_t)acc;
}
