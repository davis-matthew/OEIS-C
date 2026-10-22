/*
 * A318439  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          h(n) = Sum_{k=0..w} b_k * (i-1)^k (where i denotes the imaginary
 *          unit); a(n) is the imaginary part of h(n).
 * https://oeis.org/A318439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = ∑_{k≥0} bit_k(n) * Im((i‑1)^k) where i² = –1 and bit_k(n)
 *            is * the k‑th least significant bit of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t r = 1; /* real part of (i-1)^k, initially k = 0 */
    seq_t i = 0; /* imaginary part of (i-1)^k */

    for (idx_t k = 0; k < 64; ++k) {
        if ((n >> k) == 0)
            break;                      /* no higher bits */

        if (((n >> k) & 1ULL) != 0) {
            sum += i;                   /* accumulate imaginary part */
        }

        /* advance to the next power: (i-1)^(k+1) = (r + i*i) * (i-1) */
        seq_t r_next = -r - i;
        seq_t i_next =  r - i;
        r = r_next;
        i = i_next;
    }
    return sum;
}
