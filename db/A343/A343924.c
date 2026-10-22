/*
 * A343924  a(n) = the maximum number of times n can be multiplied by a number
 *          > 1 such that each product has distinct digits.
 * https://oeis.org/A343924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum length of a chain n, n*m, n*m^2, …, where each
 *            term has all distinct decimal digits
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum value allowed for a product – 10 digits with distinct digits. */
#include <stdbool.h>
#include <stdint.h>

#define MAXVAL 9999999999ULL          /* 10^10 – 1 */

static bool digits_unique(uint64_t x)
{
    unsigned char seen[10] = {0};
    while (x)
    {
        unsigned d = x % 10;
        if (seen[d])
            return false;
        seen[d] = 1;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Result for n <= 0 or beyond the defined range: 0 */
    if (n == 0 || n > 4938271605ULL)
        return (seq_t)0;

    /* Numbers ending in two or more zeros have a(n) = 0 */
    if ((n % 100) == 0)
        return (seq_t)0;

    uint64_t sn = (uint64_t)n;
    uint64_t max_m = MAXVAL / sn;   /* largest m such that n*m <= MAXVAL */

    /* Hard bound on m – large values are unlikely to give a longer chain.
       1 000 000 is comfortably larger than the typical optimal m. */
    if (max_m > 1000000ULL)
        max_m = 1000000ULL;

    uint64_t best = 0;

    for (uint64_t m = 2; m <= max_m; ++m)
    {
        uint64_t prod = sn * m;
        if (prod > MAXVAL || !digits_unique(prod))
            continue;

        uint64_t len = 1;
        uint64_t cur = prod;

        while (1)
        {
            uint64_t next = cur * m;
            if (next > MAXVAL)
                break;
            if (!digits_unique(next))
                break;
            ++len;
            cur = next;
        }
        if (len > best)
            best = len;
    }

    return (seq_t)best;
}
