/*
 * A367342  a(n) = n+1 for n = 1 to 8; a(n) = 100 + a(n-8) for n = 9 to 16;
 *          thereafter a(8*i+j) = 10^(i+1) + a(8*(i-1)+j) for i >= 2, 1 <= j
 *          <= 8.
 * https://oeis.org/A367342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10^i - 1)/9)*100 + j, where i = floor((n-1)/8) and j =
 *            (n-1)%8 + 2
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 72, u64 n <= 152,
 *            u128 n <= 304, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    idx_t i = (n - 1) / 8;
    idx_t j = (n - 1) % 8 + 2;

    seq_t p = pow10(i);          /* 10^i */
    seq_t term = (p - 1) / 9;    /* (10^i - 1) / 9   */
    term = term * 100;           /* multiply by 100  */
    term = term + (seq_t)j;      /* add j (2..9)     */
    return term;
}
