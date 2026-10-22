/*
 * A389220  Harshad numbers which when divided by sum of their digits, give an
 *          integer which is a non-Harshad number.
 * https://oeis.org/A389220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th integer k such that k is divisible by
 *            digit_sum(k) and (k / digit_sum(k)) is a non‑Harshad integer.
 *            The implementation performs a linear search for each requested n
 * Range:     u8 n <= 34, u16 n <= 7224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; behavior below offset is irrelevant */

    idx_t count = 0;          /* how many qualifying numbers we have found */
    seq_t num = 0;            /* current integer being tested */

    while (count < n)
    {
        ++num;                /* start checking from 1 upwards */

        seq_t s = digit_sum(num);
        if ((num % s) != 0)      /* not a Harshad number */
            continue;

        seq_t q = num / s;             /* quotient after division */
        seq_t sq = digit_sum(q);
        if ((q % sq) != 0)             /* quotient is not Harshad */
        {
            ++count;
            if (count == n)
                return num;
        }
    }

    /* This point is never reached but satisfies the compiler. */
    return num;
}
