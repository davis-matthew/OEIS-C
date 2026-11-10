/*
 * A278483  Transpose of square array A278482.
 * https://oeis.org/A278483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(s-j, j) with s, j from n = s(s+1)/2 + j, 0 <= j <= s;
 *            A(0,m) = m, A(k,m) = A(k-1, floor(m*(k+1)/k)) (A278482
 *            transposed)
 * Range:     u8 n <= 647, u16 n <= 7259, u32 n <= 7259, u64 n <= 7259,
 *            u128 n <= 7259, big any n
 * Validated: standard, pass, 107 terms through n = 6352 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static idx_t A278483_tri(idx_t s)
{
    return (s * (s + 1)) / 2;
}

seq_t a(idx_t n)
{
    idx_t s = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while (s > 0 && A278483_tri(s) > n) s--;
    while (A278483_tri(s + 1) <= n) s++;

    idx_t j = n - A278483_tri(s);
    idx_t k = s - j;
    seq_t value = (seq_t)j;
    idx_t step = k;
    while (step > 0) {
        value = (value * (seq_t)(step + 1)) / (seq_t)step;
        step--;
    }
    return value;
}
