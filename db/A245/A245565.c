/*
 * A245565  a(n) = Product_{i in row n of A245562} Pell(i+1).
 * https://oeis.org/A245565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Run‑Length Transform of Pell numbers: a(n) = Π Pell(r+1), where
 *            the run lengths r are the blocks of consecutive 1's in the
 *            binary expansion of n
 * Range:     u8 n <= 126, u16 n <= 8190, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 102 terms through n = 7912 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

static seq_t pell_value(idx_t k)
{
    /* Compute the k‑th Pell number: P(0)=0, P(1)=1, P(n)=2*P(n-1)+P(n-2). */
    seq_t p0 = 0;
    seq_t p1 = 1;
    if (k == 0)
        return p0;
    for (idx_t i = 2; i <= k; ++i)
    {
        seq_t p2 = 2 * p1 + p0;
        p0 = p1;
        p1 = p2;
    }
    return p1;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t x = n;
    while (x != 0)
    {
        if ((x & 1) != 0)
        {
            /* Count a run of consecutive 1's */
            idx_t run = 0;
            while ((x & 1) != 0)
            {
                ++run;
                x >>= 1;
            }
            result = result * pell_value(run + 1);
        }
        else
        {
            x >>= 1;
        }
    }
    return result;
}
