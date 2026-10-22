/*
 * A326775  For any n >= 0, let b >= 2 be the smallest base where n has all
 *          digits equal, say to d; a(n) = d.
 * https://oeis.org/A326775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit d of the representation of n in the minimal base b
 *            where all digits of n in base b are equal
 * Range:     u8 n <= 963, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: 0 has representation "0" in any base >=2. */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;

    /* Search for the smallest base b ≥ 2 such that the digits of sn are all equal. */
    for (idx_t b = 2; b <= n + 1; ++b)
    {
        seq_t x = sn;              /* copy of n for base‑b conversion */
        seq_t digit = (seq_t)-1;   /* placeholder for the first digit seen */
        bool ok = true;

        while (x > 0)
        {
            seq_t d = x % (seq_t)b;   /* current least significant digit */
            if (digit == (seq_t)-1)
                digit = d;            /* first digit encountered */
            else if (d != digit)
            {
                ok = false;           /* digits differ */
                break;
            }
            x /= (seq_t)b;            /* shift right in base b */
        }

        if (ok)
            return digit;             /* minimal base found */
    }

    /* Fallback: for a single‑digit case (should not occur here). */
    return sn;
}
