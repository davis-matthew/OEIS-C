/*
 * A381232  Count down from k to -k for k = 0, 1, 2, ... .
 * https://oeis.org/A381232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ⌊√n⌋ * (⌊√n⌋ + 1) - n
 * Range:     i8 n <= 9978, i16 n <= 9978, i32 n <= 9978, i64 n <= 9978,
 *            i128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers:
   returns the largest k such that k*k <= n. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = 0xFFFFFFFFULL, mid, sq;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        sq  = mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t k          = floor_sqrt_u64(n);
    seq_t k_seq      = k;
    seq_t product    = k_seq * (k_seq + (seq_t)1);
    seq_t n_seq      = (seq_t)n;
    return product - n_seq;
}
