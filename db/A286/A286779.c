/*
 * A286779  Multiplicative with a(p^e) = 2e^2 + 2.
 * https://oeis.org/A286779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (2 e^2 + 2)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is not defined in OEIS; return 0 for safety */
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            seq_t se = e;          /* cast likelihood for seq_t operations */
            seq_t term = 2 * se * se + 2; /* a(p^e) = 2e^2 + 2 */
            result *= term;
        }
    }

    if (m > 1) /* remaining prime factor with exponent 1 */
    {
        seq_t term = 2 * (seq_t)1 * (seq_t)1 + 2; /* 2*1^2 + 2 = 4 */
        result *= term;
    }

    return result;
}
