/*
 * A261302  a(n+1) = abs(a(n) - gcd(a(n), 2n+1)), a(1) = 1.
 * https://oeis.org/A261302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n ≥ 1, a(n+1) = abs( a(n) – gcd( a(n), 2*n+1 ) )
 * Range:     u8 n <= 224, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence decreases by the gcd unless a(n)=0, in which
   case a(n+1)=2*n+1. This implementation follows the
   recurrence exactly, using incremental computation. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    seq_t cur = 1;

    if (n <= 1)
        return cur;

    /* iterate from i = 1 up to n-1; at step i we hold a(i)
       and compute a(i+1). */
    for (idx_t i = 1; i < n; ++i)
    {
        if (cur == 0)
        {
            /* a(i) = 0 → a(i+1) = gcd(0, 2*i+1) = 2*i+1 */
            cur = (seq_t)(2 * i + 1);
        }
        else
        {
            seq_t term = (seq_t)(2 * i + 1);
            seq_t g = gcd(cur, term);   /* seq_gcd defined in utils/gcd.h */
            cur = cur - g;              /* difference is non‑negative */
        }
    }
    return cur;
}
