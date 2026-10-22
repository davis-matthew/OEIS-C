/*
 * A334942  a(1) = 1, a(n) = a(n-1) / gcd(a(n-1),n) if this gcd is > 1, else
 *          a(n) = 2*a(n-1) + 4.
 * https://oeis.org/A334942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = a(n-1)/gcd(a(n-1),n) if gcd(a(n-1),n) > 1 a(n) =
 *            2*a(n-1)+4 otherwise for n ≥ 2
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Index 1 is the first term. No memoisation is required; a direct
       iteration from 1 to n produces each value in O(n).  This stays
       well below the time limit for n ≈ 10 000. */
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, but safe to return 0 */

    if (n == 1)
        return (seq_t)1;

    seq_t cur = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t g = gcd(cur, (seq_t)i);
        if (g > (seq_t)1)
            cur = cur / g;
        else
            cur = cur * (seq_t)2 + (seq_t)4;
    }
    return cur;
}
