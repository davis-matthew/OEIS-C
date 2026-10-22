/*
 * A309841  If n = Sum (2^e_k) then a(n) = Product ((e_k + 2)!).
 * https://oeis.org/A309841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Product over bits of (e+2)! where n = Σ 2^e
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 58, u64 n <= 249,
 *            u128 n <= 1918, big any n
 * Helpers:   factorial, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    while (n != 0)
    {
        idx_t e = floor_log2_uint64(n);
        result = result * factorial(e + 2);
        n -= ((idx_t)1 << e);
    }
    return result;
}
