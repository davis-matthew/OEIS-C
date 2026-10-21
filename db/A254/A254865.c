/*
 * A254865  a(n) = Product_{k = 1+n-floor(n/3) .. n} k.
 * https://oeis.org/A254865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k = 1 + n - floor(n/3) .. n} k
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 23, u64 n <= 38, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* floor(n/3) */
    idx_t k = n / 3;
    /* lower bound = 1 + n - floor(n/3) = n - k + 1 */
    idx_t start = n - k + 1;

    seq_t res = 1;
    for (idx_t i = start; i <= n; ++i) {
        res *= (seq_t)i;
    }
    return res;
}
