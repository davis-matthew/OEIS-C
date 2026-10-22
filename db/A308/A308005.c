/*
 * A308005  A modified Sisyphus function: a(n) = concatenation of (number of
 *          odd digits in n) (number of digits in n) (number of even digits in
 *          n).
 * https://oeis.org/A308005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(#odd digits in n, #digits in n, #even digits in n)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow10_of(unsigned v)
{
    /* v is at most 20 for 64‑bit n, so only 1 or 2 decimal digits */
    return (v < 10) ? (seq_t)10 : (seq_t)100;
}

seq_t a(idx_t n)
{
    unsigned oddCnt = 0;      /* number of odd digits */
    unsigned digitsCnt = 0;   /* total number of decimal digits */

    idx_t tmp = n;
    if (tmp == 0)
        digitsCnt = 1;        /* 0 has one digit */
    else
    {
        while (tmp > 0)
        {
            unsigned d = (unsigned)(tmp % 10);
            if (d & 1u)               /* odd digit */
                ++oddCnt;
            ++digitsCnt;
            tmp /= 10;
        }
    }

    unsigned evenCnt = digitsCnt - oddCnt;

    seq_t result = (seq_t)oddCnt;
    result = result * pow10_of(digitsCnt) + (seq_t)digitsCnt;
    result = result * pow10_of(evenCnt)   + (seq_t)evenCnt;

    return result;
}
