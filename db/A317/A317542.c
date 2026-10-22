/*
 * A317542  Formal inverse of the period-doubling sequence A096268.
 * https://oeis.org/A317542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = 0; a(1) = 1; a(4n+1) = a(2n-1) (n≥1); a(4n+3) = a(n)
 *            (n≥0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    if ((n & 1) == 0)           /* even indices give 0 */
        return 0;
    if ((n & 3) == 1)           /* n ≡ 1 (mod 4), n > 1 */
    {
        idx_t k = (n - 1) / 4;   /* k ≥ 0, but k==0 only for n==1 handled above */
        return a((idx_t)(2 * k - 1));
    }
    /* n ≡ 3 (mod 4) */
    idx_t k = (n - 3) / 4;
    return a(k);
}
