/*
 * A288194  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 430", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A288194: Binary representation of the diagonal in Rule‑430
 *
 *  a(0) = 1, a(1) = 1, a(2) = 0
 *  a(n) = 1                     for odd n
 *  a(n) = decimal number that
 *         consists of (p‑1) ones followed by a zero,
 *         where p = 2^⌊log2(n/2)⌋  (p ≥ 2 for even n ≥ 4).
 *  The decimal digits of the binary string are returned as an integer.
 *
 *  Example: n = 8   →  p = 4 → 1110
 *           n = 16  →  p = 8 → 11111110
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Handle the initial terms */
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2)          return (seq_t)0;

    /* Odd indices return 1 */
    if ((n & (idx_t)1) != 0) return (seq_t)1;

    /* Even n ≥ 4 */
    idx_t k = n >> 1;                     /* k = n/2 */
    uint64_t m = floor_log2_uint64(k);    /* m = ⌊log2(k)⌋ */
    idx_t  p = (idx_t)1 << m;             /* p = 2^m,  p ≥ 2 */

    /* Build decimal number with (p‑1) ones followed by 0 */
    seq_t result = 0;
    for (idx_t i = 0; i < p - 1; ++i) {
        result = result * (seq_t)10 + (seq_t)1;
    }
    result = result * (seq_t)10;   /* trailing zero */

    return result;
}
