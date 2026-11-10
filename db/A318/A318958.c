/*
 * A318958  A(n, k) is a square array read in the decreasing antidiagonals,
 *          for n >= 0 and k >= 0.
 * https://oeis.org/A318958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A(n,0)=h(n), A(n,1)=h(n+1), A(n,k)=A(n,k-2)+n, with h(0)=h(1)=0,
 *            h(2)=-1, h(n)=A198442(n-2) for n>=3,
 *            A198442(m)=(2*m^2-5-3*(-1)^m)/8; read by antidiagonals with n
 *            increasing
 * Range:     i8 n <= 75, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-08)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

static seq_t A318958_A198442(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * 2 - 5;
    if ((n & 1) == 0)
        num -= 3;
    else
        num += 3;
    return num / 8;
}

static seq_t A318958_h(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (n == 2) return (seq_t)-1;
    return A318958_A198442(n - 2);
}

seq_t a(idx_t n)
{
    idx_t d = (isqrt(8 * n + 1) - 1) / 2;
    while (d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    idx_t i = n - d * (d + 1) / 2;   /* row index */
    idx_t k = d - i;                 /* column index */
    seq_t base = (k & 1) ? A318958_h(i + 1) : A318958_h(i);
    return base + (seq_t)i * (seq_t)(k / 2);
}
