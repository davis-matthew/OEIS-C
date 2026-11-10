/*
 * A353154  Sum of the even-indexed terms in the n-th row of the triangle
 *          A196020.
 * https://oeis.org/A353154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k even, k(k+1)/2 <= n, k | (n - k(k+1)/2)} (2*((n -
 *            k(k+1)/2)/k) + 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 2; k * (k + 1) / 2 <= n; k += 2) {
        idx_t diff = n - k * (k + 1) / 2;
        if (diff % k == 0) {
            sum += (seq_t)2 * (seq_t)(diff / k) + 1;
        }
    }
    return sum;
}
