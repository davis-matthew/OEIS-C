/*
 * A389091  Sum over i of the i-th digit of n raised to the i-th power.
 * https://oeis.org/A389091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = d_1^1 + d_2^2 + … + d_m^m (digits of n in decimal)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Special case n == 0: treat as single digit '0' */
    uint8_t rev[20];  /* enough for any uint64_t number */
    int len = 0;
    if (n == 0) {
        len = 1;
        rev[0] = 0;
    } else {
        uint64_t tmp = n;
        while (tmp > 0) {
            rev[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    seq_t sum = 0;
    for (int i = 0; i < len; ++i) {
        int digit = rev[len - 1 - i];      /* most significant first */
        idx_t pos = (idx_t)(i + 1);       /* positions start at 1 */
        sum += seq_pow((seq_t)digit, pos);
    }
    return sum;
}
