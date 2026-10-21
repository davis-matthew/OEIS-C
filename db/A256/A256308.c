/*
 * A256308  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 0
 *          to the digits of n written in base 8; do not convert back to base
 *          10.
 * https://oeis.org/A256308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform each octal digit of n by (d+1) mod 8 and
 *            interpret the resulting digit sequence as a decimal integer
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t and related types */

seq_t a(idx_t n)
{
    /* Extract the base‑8 digits of n (least significant first). */
    uint8_t digs[64];   /* maximum 22 digits for a 64‑bit integer, safe buffer */
    int len = 0;
    if (n == 0) {
        digs[0] = 0;
        len = 1;
    } else {
        while (n > 0) {
            digs[len++] = (uint8_t)(n & 7);   /* n % 8 */
            n >>= 3;                          /* n /= 8 */
        }
    }

    /* Transform the digits and build the decimal result. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d   = digs[i];
        uint8_t td  = (d + 1) & 7;   /* (d+1) mod 8 */
        res = res * 10 + td;
    }
    return res;
}
