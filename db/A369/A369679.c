/*
 * A369679  a(n) = Product_{k=0..n} (3^k + 4^(n-k)).
 * https://oeis.org/A369679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (3^k + 4^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>   /* required only for the build environment */

seq_t a(idx_t n) {
    seq_t pow3 = (seq_t)1;          /* 3^0 */
    seq_t pow4 = (seq_t)1;          /* 4^0 initially */
    /* compute 4^n */
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;
    }

    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        prod *= (pow3 + pow4);      /* multiply by (3^k + 4^(n-k)) */
        pow3 *= (seq_t)3;           /* next 3^k */
        pow4 /= (seq_t)4;           /* next 4^(n-k) */
    }
    return prod;
}
