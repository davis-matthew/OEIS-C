/*
 * A400896  a(n) = numerator(1 - (n^2 - n + 2)/2^n).
 * https://oeis.org/A400896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, new
 * Formula:   a(n) = numerator of (1 - (n^2 - n + 2)/2^n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* safe guard, although n ≥ 1 by spec */

    /* Compute A = n^2 - n + 2 */
    seq_t A = (seq_t)n * (seq_t)n - (seq_t)n + (seq_t)2;

    /* p = 2^n */
    seq_t p = pow_seq((seq_t)2, (int)n);

    /* numerator = 2^n - A */
    seq_t num = p - A;
    if (num == 0) return 0;

    /* Divide out all powers of two (i.e. compute the numerator of the
       reduced fraction) */
    while ((num & (seq_t)1) == 0) {
        num >>= 1;
    }
    return num;
}
