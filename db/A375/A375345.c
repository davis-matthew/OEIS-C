/*
 * A375345  a(n) = b(n, n-1) where b(n, m) = b(n, m-1) + gcd(b(n, m-1), n-m)
 *          for m > 0 with b(n, 0) = n.
 * https://oeis.org/A375345
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = b(n, n-1);
   b(n, m) = b(n, m-1) + gcd(b(n, m-1), n-m) for m>0,
   with b(n,0) = n.  This implements OEIS sequence A375345. */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined below the offset */

    seq_t b = (seq_t)n;
    for (idx_t m = 1; m < n; ++m)
    {
        seq_t d = (seq_t)(n - m);
        b += gcd(b, d);
    }
    return b;
}
