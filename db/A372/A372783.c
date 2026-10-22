/*
 * A372783  In the ternary expansion of n: keep the initial digit, and then
 *          replace 0's with 1's and 1's with 0's.
 * https://oeis.org/A372783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = apply ternary digit replacement: keep the most
 *            significant digit, replace each 0 with 1 and each 1 with 0 in
 *            the remaining digits
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used internally for idx_t manipulation */

seq_t a(idx_t n)
{
    /* Handle zero specially – its ternary form is “0” and stays “0”. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Record the ternary digits from least‑significant to most‑significant.
       For a 64‑bit integer fewer than 64 digits are needed. */
    unsigned char digits[64];
    int pos = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[pos++] = (unsigned char)(tmp % 3u);
        tmp /= 3u;
    }

    /* Build the resulting number from the most‑significant digit down,
       applying the digit swap rule on all but the first digit. */
    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i) {
        seq_t d = digits[i];
        if (i == pos - 1) {               /* most significant digit – unchanged */
            result = result * 3 + d;
        } else {                          /* other digits – 0↔1, 2 unchanged */
            seq_t nd = (d == 0 ? 1 : (d == 1 ? 0 : 2));
            result = result * 3 + nd;
        }
    }
    return result;
}
