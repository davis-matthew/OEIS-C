/*
 * A374009  a(n) = (1 + (n+7)^2 - (n-8)*(-1)^n)/2.
 * https://oeis.org/A374009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+7)^2 - (n-8)*(-1)^n)/2
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (n+7)^2 */
    seq_t base = (seq_t)(n + 7);
    seq_t base_sq = base * base;

    /* Sum = base_sq + 1  */
    seq_t sum = base_sq + 1;

    /* Adjust sum depending on parity of n and the sign of (n-8) */
    if ((n & 1ULL) == 0ULL)        /* even n: subtract (n-8) */
    {
        if (n >= 8ULL)
            sum -= (seq_t)(n - 8ULL);
        else
            sum += (seq_t)(8ULL - n);   /* subtract a negative number */
    }
    else                          /* odd n: add (n-8) */
    {
        if (n >= 8ULL)
            sum += (seq_t)(n - 8ULL);
        else
            sum -= (seq_t)(8ULL - n);   /* add a negative number */
    }

    /* Final result */
    return sum / 2;
}
