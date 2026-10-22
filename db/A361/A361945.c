/*
 * A361945  If the ternary expansion of n starts with the digit 1, then
 *          replace 2's by 0's and vice versa; if the ternary expansion of n
 *          starts with the digit 2, then replace 1's by 0's and vice versa;
 *          a(0) = 0.
 * https://oeis.org/A361945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform ternary digits of n where if the first ternary
 *            digit is 1, replace 2↔0; if the first ternary digit is 2,
 *            replace 1↔0; a(0) = 0
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* store ternary digits of n (least significant first) */
    uint8_t digs[64];
    size_t len = 0;
    seq_t tmp = (seq_t)n;
    while (tmp) {
        digs[len++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }

    /* first (most significant) digit determines the mapping */
    uint8_t first = digs[len - 1];

    seq_t res = 0;
    for (ptrdiff_t i = (ptrdiff_t)len - 1; i >= 0; --i) {
        uint8_t d = digs[i];
        uint8_t d2;
        if (first == 1) {
            if (d == 2)      d2 = 0;
            else if (d == 0) d2 = 2;
            else             d2 = d;   /* d == 1 */
        } else { /* first == 2 */
            if (d == 1)      d2 = 0;
            else if (d == 0) d2 = 1;
            else             d2 = d;   /* d == 2 */
        }
        res = res * 3 + d2;
    }

    return res;
}
