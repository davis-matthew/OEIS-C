/*
 * A327703  a(n) = (binomial(n,floor(n/2)))/(greatest common divisor of all
 *          numbers in n-th row of Pascal's triangle excluding 1 and n).
 * https://oeis.org/A327703
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,floor(n/2)) / gcd of all C(n,k) for 2 <= k <=
 *            n-2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0; /* values below the offset are undefined */

    idx_t k;
    seq_t c = 1;      /* C(n,0) */
    seq_t mid = 0;    /* C(n, floor(n/2)) */
    seq_t g = 0;      /* running gcd */
    idx_t h = n >> 1; /* floor(n/2) */

    for (k = 1; k <= n; ++k)
    {
        /* C(n,k) = C(n,k-1) * (n-k+1)/k */
        c = c * (seq_t)(n - k + 1) / (seq_t)k;

        if (k == h)
            mid = c;

        if (k >= 2 && k <= n - 2)
        {
            if (g == 0)
                g = c;
            else
                g = gcd(g, c);
        }
    }

    if (g == 0)
        return 0; /* should not happen for n >= 4 */

    return mid / g;
}
