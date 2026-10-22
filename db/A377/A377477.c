/*
 * A377477  Consider the nonadjacent form for n, then reverse the digits,
 *          leaving any trailing zeros alone, and take the absolute value.
 * https://oeis.org/A377477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = | Σ_i d_i * 2^i |, where (d_i) is obtained from the
 *            non‑adjacent form of n: reverse the digits except the trailing
 *            zeros (least significant zeros are left unchanged). The NAF
 *            digits are 0, 1, or -1
 * Range:     u8 n <= 170, u16 n <= 9579, u32 n <= 9579, u64 n <= 9579,
 *            u128 n <= 9579, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* produce the NAF of n in little‑endian order (least significant first) */
    signed char buf[128];
    size_t len = 0;
    idx_t temp = n;

    while (temp > 0) {
        int digit;
        if (temp & 1) {               /* odd */
            int mod4 = (int)(temp & 3);   /* temp % 4 */
            digit = 2 - mod4;              /* 1 or -1 */
            temp = (temp - digit) >> 1;
        } else {                      /* even */
            digit = 0;
            temp >>= 1;
        }
        buf[len++] = (signed char)digit;
    }
    if (len == 0) {                  /* n == 0 */
        buf[0] = 0;
        len = 1;
    }

    /* find the index of the last non‑zero digit */
    size_t last = 0;
    int found = 0;
    for (size_t i = len; i-- > 0; ) {
        if (buf[i] != 0) {
            last = i;
            found = 1;
            break;
        }
    }
    if (!found)                     /* all zeros */
        return 0;

    /* count trailing zeros (least significant zeros) */
    size_t t = 0;
    while (t < len && buf[t] == 0)
        ++t;

    /* accumulate value from the reversed part */
    seq_t posSum = 0;
    seq_t negSum = 0;
    for (size_t k = 0; k <= last - t; ++k) {
        int digit = buf[last - k];
        if (digit == 0)
            continue;
        seq_t val = ((seq_t)1) << (t + k);
        if (digit > 0)
            posSum += val;
        else
            negSum += val;
    }

    return (posSum >= negSum) ? (posSum - negSum) : (negSum - posSum);
}
