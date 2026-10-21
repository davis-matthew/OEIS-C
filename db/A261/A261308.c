/*
 * A261308  a(n+1) = abs(a(n) - gcd(a(n), 8n+7)), a(1) = 1.
 * https://oeis.org/A261308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = abs(a(n) − gcd(a(n), 8n+7))
 * Range:     u8 n <= 33, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence initial value: a(1) = 1 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                       /* undefined for n<1, safe default */

    seq_t prev = (seq_t)1;                     /* a(1) */
    if (n == 1)
        return prev;

    for (idx_t i = 2; i <= n; ++i)             /* compute a(i) from a(i-1) */
    {
        seq_t val = (seq_t)8 * ((seq_t)(i - 1)) + (seq_t)7; /* 8*(i-1)+7 */
        seq_t g   = gcd(prev, val);                  /* gcd(a(i-1), 8*(i-1)+7) */
        seq_t next;
        if (prev >= g)
            next = prev - g;
        else
            next = g - prev;                       /* absolute difference */
        prev = next;
    }
    return prev;
}
