/*
 * A342147  Numbers m such that m + (m+1) = 2m + 1 shares at least one digit
 *          with m+1.
 * https://oeis.org/A342147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m such that digits(m+1) share at least
 *            one digit with digits(2m+1)
 * Range:     u8 n <= 53, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return a bitmask of the decimal digits present in x
   (bit d is set iff digit d occurs). */
static unsigned digit_mask(seq_t x)
{
    unsigned mask = 0;
    if (x == 0)                /* special case: 0 has digit 0 */
        return 1u;             /* 1 << 0 */

    while (x != 0)
    {
        unsigned d = (unsigned)(x % 10);  /* digit 0‑9 */
        mask |= 1u << d;
        x /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    idx_t   seen  = 0;  /* how many terms found so far */
    seq_t   m     = 0;  /* candidate m */

    for (;;)
    {
        seq_t m1          = m + 1;        /* m+1   */
        seq_t two_m_plus1 = m * 2 + 1;   /* 2m+1  */

        unsigned mask1 = digit_mask(m1);
        unsigned mask2 = digit_mask(two_m_plus1);

        if (mask1 & mask2)       /* share at least one digit? */
        {
            ++seen;
            if (seen == n)
                return m;        /* found the nth term */
        }
        ++m;                      /* next candidate */
    }
}
