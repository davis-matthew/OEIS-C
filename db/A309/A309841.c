/*
 * A309841  If n = Sum (2^e_k) then a(n) = Product ((e_k + 2)!).
 * https://oeis.org/A309841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Product over bits of (e+2)! where n = Σ 2^e
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 58, u64 n <= 249,
 *            u128 n <= 1918, big any n
 * Validated: standard, pass, 57 terms through n = 9622 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    while (n != 0)
    {
        idx_t e = ilog2(n);
        result = result * factorial(e + 2);
        n -= ((idx_t)1 << e);
    }
    return result;
}
