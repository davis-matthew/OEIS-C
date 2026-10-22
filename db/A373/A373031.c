/*
 * A373031  Expansion of Sum_{k>=1} (-1)^(k+1) * k^2 * x^(k^2) / (1 - x^k).
 * https://oeis.org/A373031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(d+1) * d^2
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned integers.
   Uses binary search and avoids overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, r = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= n / mid) {   /* mid*mid <= n without overflow */
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

/* Sequence A373031 */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1, but callers should not
       request those values. */
    seq_t sum = 0;
    idx_t r = isqrt_u64(n);       /* floor(sqrt(n)) */

    for (idx_t d = 1; d <= r; ++d)
    {
        if (n % d == 0)
        {
            /* sign = (-1)^(d+1): +1 if d is odd, -1 if d is even */
            seq_t sign = (d & 1) ? 1 : -1;
            seq_t d2 = (seq_t)d * (seq_t)d;
            sum += sign * d2;
        }
    }
    return sum;
}
