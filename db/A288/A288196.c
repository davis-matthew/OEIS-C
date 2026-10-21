/*
 * A288196  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 430", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(odd)=1, a(2k)=2^p-2 where p is the largest power of 2
 *            ≤ k
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   big_pow, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* a(0) = 1 */
        return (seq_t)1;

    if (n & 1)                  /* odd indices */
        return (seq_t)1;

    /* Even n ≥ 2 */
    idx_t k = n >> 1;           /* k = n/2  */
    idx_t m = floor_log2_uint64(k);  /* m = ⌊log₂(k)⌋ */
    idx_t p = ((idx_t)1) << m; /* p = largest power of two ≤ k */
    seq_t val = big_pow((seq_t)2, (seq_t)p); /* 2^p */
    return val - (seq_t)2;     /* subtract 2 */
}
