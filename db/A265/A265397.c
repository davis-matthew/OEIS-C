/*
 * A265397  a(n) = n - A264977(n).
 * https://oeis.org/A265397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A264977(n), where A264977(0)=0, A264977(1)=1,
 *            A264977(2n)=2*A264977(n), A264977(2n+1)=A264977(n) XOR
 *            A264977(n+1)
 * Range:     u8 n <= 274, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 105 terms through n = 7853 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A265397_stern2(idx_t n)
{
    seq_t x = 0, y = 1; /* (A(m), A(m+1)) for m = 0 */
    int i;
    for (i = 63; i >= 0; i--)
    {
        seq_t t = x ^ y;
        if ((n >> i) & 1ULL)
        {
            x = t;
            y = y * 2;
        }
        else
        {
            x = x * 2;
            y = t;
        }
    }
    return x;
}

seq_t a(idx_t n)
{
    return (seq_t)n - A265397_stern2(n);
}
