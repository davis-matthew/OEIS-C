/*
 * A351653  a(n) is the concatenation of the length of each run of digits in
 *          the decimal representation of n.
 * https://oeis.org/A351653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the lengths of each run of digits in the
 *            decimal representation of n
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case for n = 0 */
    if (n == 0)
        return 1;

    /* n fits in idx_t (uint64_t).  A 64‑bit number has at most 20
       decimal digits, so a fixed 21‑byte buffer is sufficient. */
    uint8_t digits[21];
    idx_t t = n;
    int d = 0;
    while (t > 0) {
        digits[d++] = t % 10;
        t /= 10;
    }

    /* digits[0 .. d-1] contain the digits of n in reverse order
       (least significant first).  We now scan them from most
       significant to least to identify runs. */
    seq_t result = 0;
    uint8_t current = digits[d - 1];
    int run_len = 1;

    for (int i = d - 2; i >= 0; --i) {
        uint8_t digit = digits[i];
        if (digit == current) {
            /* Continue the current run */
            run_len++;
        } else {
            /* Finish the current run and append its length to the result */
            int len = 0;
            int tmp = run_len;
            while (tmp > 0) {
                len++;
                tmp /= 10;
            }
            for (int k = 0; k < len; ++k)
                result *= 10;
            result += run_len;
            /* Start a new run */
            current = digit;
            run_len = 1;
        }
    }

    /* Handle the final run */
    int len = 0;
    int tmp = run_len;
    while (tmp > 0) {
        len++;
        tmp /= 10;
    }
    for (int k = 0; k < len; ++k)
        result *= 10;
    result += run_len;

    return result;
}
