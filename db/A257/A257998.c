/*
 * A257998  Partial sums of A188967.
 * https://oeis.org/A257998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A188967(k), where A188967(1) = 0,
 *            A188967(3k-2) = A188967(k), A188967(n) = 1 - A188967(n -
 *            floor((n+2)/3)) otherwise
 * Range:     u8 n <= 501, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9729 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A188967_bit(idx_t n)
{
    unsigned val = 0;
    while (n > 1)
    {
        if (n % 3 == 1)
        {
            n = (n + 2) / 3;
        }
        else
        {
            val ^= 1;
            idx_t fl = (n + 2) / 3;
            n = n - fl;
        }
    }
    return val;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    for (k = 1; k <= n; k++)
    {
        if (A188967_bit(k))
            s++;
    }
    return s;
}
