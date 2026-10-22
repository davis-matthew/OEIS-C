/*
 * A350709  Modified Sisyphus function of order 3: a(n) is the concatenation
 *          of the number of digits in n with the number of digits of n
 *          congruent to k modulo 3 for each k from 0 to 2 in turn.
 * https://oeis.org/A350709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate(d(n), #0(n), #1(n), #2(n)) where d(n) is the
 *            number of decimal digits of n and #k(n) counts digits of n
 *            congruent to k modulo 3 (k = 0,1,2)
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_len_int.h"

/* Helper: 10^e as a seq_t value.  e is small (≤ 2 in this problem). */
static seq_t pow10_int(int e)
{
    seq_t r = 1;
    for (int i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Handle n = 0 specially: digits = 1, one digit 0 (≡0 mod 3). */
    int d, c[3];
    if (n == 0)
    {
        d = 1;
        c[0] = 1;  /* digit 0 */
        c[1] = 0;
        c[2] = 0;
    }
    else
    {
        d = 0;
        c[0] = c[1] = c[2] = 0;
        idx_t tmp = n;
        while (tmp > 0)
        {
            int digit = tmp % 10;
            d++;
            c[digit % 3]++;
            tmp /= 10;
        }
    }

    seq_t res = d;  /* start with the digit count */

    /* Append counts in order 0,1,2 */
    res = res * pow10_int(digit_len_int(c[0])) + c[0];
    res = res * pow10_int(digit_len_int(c[1])) + c[1];
    res = res * pow10_int(digit_len_int(c[2])) + c[2];

    return res;
}
