/*
 * A396834  Lower (2/3,1/3) midsequence of (floor[n/2]^2) and
 *          (ceiling[n/2]^2); see Comments.
 * https://oeis.org/A396834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2* floor(n/2)^2 + ceil(n/2)^2)/3)
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute floor(n/2) and ceil(n/2) using integer arithmetic */
    idx_t half = n >> 1;            /* floor(n/2) */
    idx_t ceil = n - half;          /* ceil(n/2)  */

    /* squares in seq_t to avoid overflow in intermediate types */
    seq_t half_sq = (seq_t)half * (seq_t)half;
    seq_t ceil_sq  = (seq_t)ceil * (seq_t)ceil;

    /* result = floor((2*half_sq + ceil_sq)/3) */
    return ( (seq_t)2 * half_sq + ceil_sq ) / (seq_t)3;
}
