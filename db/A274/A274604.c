/*
 * A274604  Running sum of Noergaard's "infinity sequence" A004718.
 * https://oeis.org/A274604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(2n) = n - s(n) * a(2n+1) = n + 1 * where s(n) is Noergaard's
 *            "infinity sequence": * s(0) = 0 * s(2n) = -s(n) * s(2n+1) = s(n)
 *            + 1 * This implementation evaluates s(n) by processing bits from
 *            * most‑significant to least‑significant, ensuring correctness
 *            for * all n >= 0
 * Range:     u8 n <= 505, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9304 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t _s_of(idx_t n)
{
    int64_t s = 0;
    if (n == 0)
        return 0;

    /* Find the most significant set bit (mask). */
    idx_t mask = 1;
    while (mask <= (n >> 1))
        mask <<= 1;

    /* Process bits from MSB down to LSB. */
    for (idx_t m = mask; m > 0; m >>= 1)
    {
        if (n & m)
            s = s + 1;   /* bit 1 */
        else
            s = -s;      /* bit 0 */
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n & (idx_t)1ULL)          /* odd n: a(2k+1) = k+1 */
    {
        return (seq_t)((n >> 1) + (idx_t)1);
    }
    else                          /* even n: a(2k) = k - s(k) */
    {
        idx_t k = n >> 1;
        int64_t s = _s_of(k);
        int64_t val = (int64_t)k - s;   /* guaranteed non‑negative */
        return (seq_t)val;
    }
}
