/*
 * A249098  Position of n^6 in the ordered union of {h^6, h >=1} and {3*k^6, k
 *          >=1}.
 * https://oeis.org/A249098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n / 3^(1/6))
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for n<1 */

    /* use 128‑bit arithmetic for the intermediate calculations */
    __int128 n128 = n;
    __int128 n6 = 1;
    for (int i = 0; i < 6; ++i) n6 *= n128;   /* n^6 */

    __int128 x = n6 / 3;          /* floor(n^6 / 3) */

    /* binary search for largest k such that k^6 <= x */
    idx_t low = 0;
    idx_t high = n;   /* floor(n / 3^(1/6)) < n */
    idx_t best = 0;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);

        __int128 mid6 = 1;
        for (int i = 0; i < 6; ++i) mid6 *= (__int128)mid;

        if (mid6 <= x)
        {
            best = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    return (seq_t)n + best;
}
