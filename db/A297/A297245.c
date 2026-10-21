/*
 * A297245  Total variation of base-15 digits of n; see Comments.
 * https://oeis.org/A297245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i=1}^{k-1} |d_i - d_{i-1}| where d_i are the base‑15
 *            digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (total variation of the base‑15 representation). */

#include <stdint.h>  /* for uint8_t */

seq_t a(idx_t n)
{
    /* Store the base‑15 digits (least significant first).  A 64‑bit integer has at most 17 digits. */
    static uint8_t digits[64];
    uint8_t count = 0;
    idx_t temp = n;

    /* Special case: if n==0 (not part of the OEIS offset) we still return 0 */
    if (temp == 0) {
        digits[0] = 0;
        count = 1;
    } else {
        while (temp > 0) {
            digits[count++] = (uint8_t)(temp % 15);
            temp /= 15;
        }
    }

    seq_t sum = 0;
    uint8_t i;
    for (i = 1; i < count; ++i) {
        uint8_t d1 = digits[i];
        uint8_t d0 = digits[i - 1];
        if (d1 > d0) {
            sum += (seq_t)(d1 - d0);
        } else {
            sum += (seq_t)(d0 - d1);
        }
    }
    return sum;
}
