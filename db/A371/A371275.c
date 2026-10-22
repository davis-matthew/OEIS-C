/*
 * A371275  a(n) is the number of runs in the balanced ternary expansion of n.
 * https://oeis.org/A371275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs in the balanced ternary expansion of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0 (a(0) = 0) */
    if (n == 0)
        return (seq_t)0;

    /* Balanced ternary digits are in the range {-1,0,1}.  For
       numbers up to 2^64-1 we need at most 70 digits (log_3(2^64) < 70). */
    int digits[70];
    int len = 0;

    /* Convert n to balanced ternary, storing least‑significant digits first. */
    while (n > 0)
    {
        idx_t r = n % 3;
        n /= 3;
        int d;
        if (r == 2)
        {
            d = -1;
            n += 1;            /* carry to the next higher digit */
        }
        else
        {
            d = (int)r;        /* r is 0 or 1 */
        }
        digits[len++] = d;
    }

    /* Count runs in the most‑significant‑digit order. */
    seq_t runs = 1;                 /* first non‑zero digit starts a run */
    int prev = digits[len - 1];     /* most significant digit */
    for (int i = len - 2; i >= 0; --i)
    {
        int cur = digits[i];
        if (cur != prev)
            ++runs;
        prev = cur;
    }

    return runs;
}
