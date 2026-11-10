/*
 * A286388  Compound filter ("discard the smallest prime factor" & "number of
 *          trailing 1-bits in base-2"): a(n) = P(A032742(n), A001511(1+n)),
 *          where P(n,k) is sequence A000027 used as a pairing function, with
 *          a(1) = 0.
 * https://oeis.org/A286388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = (2 + (x+y)^2 - x - 3*y)/2 with x =
 *            A032742(n), y = A001511(n+1)
 * Range:     u8 n <= 31, u16 n <= 661, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9786 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A286388_lpd(uint64_t nn)
{
    if (nn <= 1)
        return 1;
    uint64_t spf = nn;
    if ((nn % 2) == 0)
        spf = 2;
    else
    {
        for (uint64_t d = 3; d <= nn / d; d += 2)
        {
            if ((nn % d) == 0)
            {
                spf = d;
                break;
            }
        }
    }
    return nn / spf;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t x = (seq_t)A286388_lpd((uint64_t)n);

    uint64_t m = (uint64_t)n + 1;
    unsigned r = 1;
    while ((m & 1) == 0)
    {
        r++;
        m >>= 1;
    }
    seq_t y = (seq_t)r;

    seq_t s = x + y;
    seq_t t = s * s + 2;
    t = t - x - 3 * y;
    return t / 2;
}
