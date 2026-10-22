/*
 * A308003  A modified Sisyphus function: a(n) = concatenation of (number of
 *          even digits in n) (number of digits in n) (number of odd digits in
 *          n).
 * https://oeis.org/A308003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of (number of even digits in n)(number of
 *            digits in n)(number of odd digits in n)
 * Range:     u8 n <= 199, u16 n <= 8543, u32 n <= 8543, u64 n <= 8543,
 *            u128 n <= 8543, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Count decimal digits of a 64‑bit unsigned value */
static inline int digits_of_uint64(idx_t v)
{
    if (v == 0) return 1;
    int d = 0;
    while (v) {
        d++;
        v /= 10;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Count even, odd and total digits of n. */
    idx_t tmp = n;
    unsigned even_cnt = 0;
    unsigned odd_cnt = 0;
    unsigned digit_cnt = 0;

    if (tmp == 0) {
        digit_cnt = 1;
        even_cnt = 1;          /* 0 is an even digit */
    } else {
        while (tmp > 0) {
            unsigned d = tmp % 10;
            digit_cnt++;
            if ((d & 1u) == 0u)
                even_cnt++;
            else
                odd_cnt++;
            tmp /= 10;
        }
    }

    /* Build the concatenation: even_digits | total_digits | odd_digits */
    seq_t res = (seq_t)even_cnt;

    int d_digits = digits_of_uint64(digit_cnt);
    res = res * pow10((idx_t)d_digits) + (seq_t)digit_cnt;

    int o_digits = digits_of_uint64(odd_cnt);
    res = res * pow10((idx_t)o_digits) + (seq_t)odd_cnt;

    return res;
}
