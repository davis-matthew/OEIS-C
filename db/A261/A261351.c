/*
 * A261351  Inverse permutation to A084385.
 * https://oeis.org/A261351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   inverse permutation of A084385. For n >= 8: a(n) = n+2 if n mod
 *            4 = 2, n+1 if n mod 4 = 0, n-1 if n mod 4 = 3, n-2 if n mod 4 =
 *            1; initial values a(1..7) = 1,2,4,3,6,8,5
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8912 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 7) {
        static const seq_t init[7] = {1, 2, 4, 3, 6, 8, 5};
        return init[n - 1];
    }
    seq_t t = (seq_t)n;
    switch (n & 3ULL) {
        case 0:  return t + (seq_t)1;
        case 1:  return t - (seq_t)2;
        case 2:  return t + (seq_t)2;
        default: return t - (seq_t)1;
    }
}
