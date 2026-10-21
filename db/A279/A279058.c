/*
 * A279058  Least number k = concat(x,y) such that k = n*(x+y) - x * y , -1 if
 *          such a number does not exist.
 * https://oeis.org/A279058
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k = concat(x,y) such that k = n * (x + y) – x * y ;
 *            -1 if none
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)-1;           /* offset starts at 3 */

    seq_t best = 0;
    seq_t pow10 = 1;
    const int MAX_D = 18;                  /* safe upper bound for powers of 10 */

    for (int d = 1; d <= MAX_D; ++d)
    {
        pow10 *= 10;                         /* 10^d */

        /* If we already have a candidate smaller than the next base,
           further powers cannot produce a smaller k. */
        if (best != 0 && pow10 > best) break;

        /* Scan all possible y values for this d. */
        for (seq_t y = 0; y < pow10; ++y)
        {
            /* Special trivial solution when n == pow10 and y == 0. */
            if (n == (idx_t)pow10 && y == 0)
            {
                seq_t k = pow10;            /* x = 1 */
                if (best == 0 || k < best) best = k;
                continue;
            }

            /* Compute denominator and numerator using signed 128‑bit
               arithmetic to safely handle signs. */
            __int128 denom = (__int128)pow10 - (__int128)n + (__int128)y;   /* = 10^d - n + y */
            if (denom == 0) continue;           /* would need y == n - 10^d, handled above */

            __int128 num = (__int128)y * (__int128)(n - 1);                 /* = y * (n‑1) */

            if (num % denom != 0) continue; /* x must be integer */

            __int128 x = num / denom;          /* x = y*(n-1)/(10^d - n + y) */
            if (x <= 0) continue;              /* x must be positive */

            seq_t k = pow10 * (seq_t)x + y;      /* k = 10^d * x + y */

            if (best == 0 || k < best) best = k;
        }
    }

    return (best == 0) ? (seq_t)-1 : best;
}
