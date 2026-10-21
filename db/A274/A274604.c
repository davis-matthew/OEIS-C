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
 * Helpers:   _s_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_s_of.h"

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
