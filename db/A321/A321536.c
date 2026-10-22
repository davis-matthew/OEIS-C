/*
 * A321536  Write n in base 10, lengthen all the runs of successive identical
 *          digits by 1.
 * https://oeis.org/A321536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   For every maximal run of identical decimal digits in n, * output
 *            that digit repeated one more time. Thus if n=10, a(n)=1100, * if
 *            n=11, a(n)=111
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 simply outputs 0. */
    if (n == 0)
        return (seq_t)0;

    /* Extract decimal digits of n into an array (most‑significant first). */
    uint8_t digits[20];      /* sufficient for any 64‑bit integer */
    int len = 0;
    idx_t temp = n;
    while (temp > 0)
    {
        digits[len++] = (uint8_t)(temp % 10);
        temp /= 10;
    }
    /* Reverse to obtain most‑significant‑digit first order. */
    for (int i = 0; i < len / 2; ++i)
    {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Build the resulting number by repeating each run's digit one more time. */
    seq_t result = 0;
    for (int i = 0; i < len; )
    {
        uint8_t d = digits[i];
        int j = i + 1;
        while (j < len && digits[j] == d)
            ++j;
        int run_len = j - i;          /* original length of this run   */
        int reps    = run_len + 1;    /* how many times to output the digit */

        for (int k = 0; k < reps; ++k)
            result = result * 10 + d;

        i = j;
    }
    return result;
}
