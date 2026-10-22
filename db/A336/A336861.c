/*
 * A336861  a(n) = ceiling((n-1-sqrt(n+1))/2).
 * https://oeis.org/A336861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n-1-sqrt(n+1))/2)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values.
   Returns floor(sqrt(n)). Uses 128‑bit arithmetic to avoid overflow. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;          /* high is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n <= 2)            /* a(1)=a(2)=0, handled early */
        return 0;

    /* floor(sqrt(n+1)) */
    idx_t s = isqrt_u64(n + 1);

    /* difference (may be negative for very small n) */
    int64_t diff = (int64_t)(n - 1) - (int64_t)s;
    if (diff <= 0)
        return 0;

    /* diff > 0, safe to cast to seq_t */
    seq_t d = (seq_t)diff;

    if ((d & 1) == 0)      /* even diff */
        return d / 2;
    else                   /* odd diff */
        return d / 2 + 1;
}
