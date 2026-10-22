/*
 * A327706  The minimal size of a partition lambda of n such that every
 *          partition of n with at most 6 parts can be obtained by coalescing
 *          the parts of lambda.
 * https://oeis.org/A327706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(floor(5*n/6)), a(0) = 0 (k = 6)
 * Range:     u8 n <= 51, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t m = n;          /* remaining part to reduce */
    seq_t res = 0;        /* current value of a(n) */

    /* iterate while the reduced value is non‑zero */
    while (m > 0) {
        /* compute floor(5*m/6) without overflow in idx_t */
        seq_t temp = (seq_t)m * 5;    /* safe: temp is at least as wide as seq_t */
        m = (idx_t)(temp / 6);       /* cast back to idx_t for the loop condition */
        res++;                       /* increment count for this recursion step */
    }
    return res;
}
