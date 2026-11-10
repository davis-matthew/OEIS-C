/*
 * A367827  Antidiagonal sums of A367825.
 * https://oeis.org/A367827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..n} A367825(n-k, k), where A367825(i,j) =
 *            denominator of (R(i)-j)/(i+j), R = digit reversal, A367825(0,0)
 *            = 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9965 (2026-11-10)
 * Helpers:   gcd, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/reverse_digits_idx.h"

static seq_t A367827_cell(idx_t i, idx_t j) {
    if (i == 0 && j == 0) return (seq_t)1;
    seq_t rev_i = (seq_t)reverse_digits_idx(i);
    seq_t jj = (seq_t)j;
    seq_t num = rev_i >= jj ? rev_i - jj : jj - rev_i;
    seq_t denom = (seq_t)i + (seq_t)j;
    seq_t g = gcd(num, denom);
    return denom / g;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; j++) {
        sum += A367827_cell(n - j, j);
    }
    return sum;
}
