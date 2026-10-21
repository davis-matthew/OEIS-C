/*
 * A244306  Table T(n,k), n>=1, k>=1, read by antidiagonals: T(n,k) = number
 *          of equivalence classes of ways of placing two 1 X 1 tiles in an n
 *          X k rectangle under all symmetry operations of the rectangle.
 * https://oeis.org/A244306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   T(n,k) computed from empirical expression: T(n,k) = (4*k*k*n*n +
 *            2*k*k + 8*k*n + 2*n*n - 4*k - 4*n - 1 - (2*k*k - 4*k - 1)*(-1)^n
 *            - (2*n*n - 4*n - 1)*(-1)^k - (-1)^k*(-1)^n) / 32 Sequence
 *            A244306 is read by antidiagonals of T(n,k)
 * Range:     u8 n <= 82, u16 n <= 1402, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Map linear index n (1‑based) to table coordinates (i,j) via antidiagonals. */
    idx_t s = 1;      /* current diagonal (1‑based) */
    idx_t tri = 0;    /* accumulated terms up to previous diagonal */

    while (tri < n) {   /* find the diagonal that contains index n */
        tri += s;       /* number of terms up to diagonal s */
        ++s;
    }

    /* Number of terms before diagonal s is tri - (s-1) = (s-2)*(s-1)/2. */
    idx_t before = (s - 2) * (s - 1) / 2;
    idx_t offset = n - before - 1;

    idx_t i = offset + 1;          /* nDim of rectangle (rows)  */
    idx_t j = s - i;               /* kDim of rectangle (cols)  */

    /* Compute T(i,j) using the empirical formula. */
    int64_t nd = (int64_t)i;        /* cast to signed 64‑bit for safety */
    int64_t kd = (int64_t)j;

    int64_t term1 = 4 * kd * kd * nd * nd;
    int64_t term2 = 2 * kd * kd;
    int64_t term3 = 8 * kd * nd;
    int64_t term4 = 2 * nd * nd;
    int64_t term5 = -4 * kd;
    int64_t term6 = -4 * nd;
    int64_t term7 = -1;

    int64_t sign_n = (nd & 1) ? -1 : 1;   /* (-1)^n */
    int64_t sign_k = (kd & 1) ? -1 : 1;   /* (-1)^k */

    int64_t term8 = - ( (2 * kd * kd - 4 * kd - 1) * sign_n );
    int64_t term9 = - ( (2 * nd * nd - 4 * nd - 1) * sign_k );
    int64_t term10 = - (sign_k * sign_n);

    int64_t num = term1 + term2 + term3 + term4
                 + term5 + term6 + term7
                 + term8 + term9 + term10;

    return (seq_t)(num / 32);
}
