/*
 * A391585  Numbers k such that 3*k = A048720(m,k) for some m, where A048720
 *          is carryless base-2 multiplication.
 * https://oeis.org/A391585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest integer k such that A048720(m,k)=3*k
 *            (carryless base‑2 multiplication)
 * Range:     u8 n <= 50, u16 n <= 3192, u32 n <= 8903, u64 n <= 8903,
 *            u128 n <= 8903, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Check whether divisor | dividend in GF(2) polynomial sense.
   Returns true iff dividend mod divisor == 0 (performed only for divisor > 0). */
static bool divisible(seq_t divisor, seq_t dividend) {
    if (divisor == 0)               /* not used for k=0 but kept for safety */
        return dividend == 0;
    unsigned div_len = bitlen_seq_t(divisor);

    while (dividend != 0) {
        unsigned divv_len = bitlen_seq_t(dividend);
        if (divv_len < div_len)
            break;
        unsigned shift = divv_len - div_len;
        dividend ^= divisor << shift;    /* polynomial subtraction = XOR */
    }
    return dividend == 0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;

    while (true) {
        if (k == 0 || divisible(k, k * 3u)) {   /* k=0 is always a term */
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
