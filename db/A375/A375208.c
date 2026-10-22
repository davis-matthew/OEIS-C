/*
 * A375208  Modified Sisyphus function of order 5: a(n) is the concatenation
 *          of the number of digits in n with the number of digits of n
 *          congruent to k modulo 5 for each k from 0 to 4 in turn.
 * https://oeis.org/A375208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the number of digits of n * with the
 *            counts of digits congruent to k modulo 5 * for k = 0..4
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t d = 0;
    seq_t cnt[5] = {0, 0, 0, 0, 0};

    idx_t t = n;
    if (t == 0) {
        d = 1;
        cnt[0] = 1;          /* digit 0 contributes to remainder 0 */
    } else {
        while (t > 0) {
            unsigned digit = t % 10;
            d++;
            cnt[digit % 5]++;   /* count modulo 5 */
            t /= 10;
        }
    }

    seq_t res = d;            /* start with total digit count */
    for (int k = 0; k < 5; ++k) {
        seq_t part = cnt[k];
        int part_digits = digit_len(part);      /* number of decimal digits */
        seq_t mult = pow10(part_digits);        /* 10^part_digits */
        res = res * mult + part;                /* concatenate */
    }
    return res;
}
