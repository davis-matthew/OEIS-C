/*
 * A288803  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 507", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1 a(1)=3 for odd n ≥3: a(n) = 2^(n+1) – 2 for even n ≥2:
 *            let k = ⌊log₂(n+2)⌋ – 1 a(n) = 2^(2^k) – 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    if (n & 1UL)                 /* odd n (≥3) */
    {
        seq_t exp = (seq_t)(n + 1);
        seq_t val = big_pow((seq_t)2, exp);
        return val - (seq_t)2;
    }
    else                         /* even n (≥2) */
    {
        idx_t m = floor_log2_uint64(n + 2);
        idx_t k = m - 1;        /* k ≥ 1 */
        seq_t exp = (seq_t)1 << k;     /* exp = 2^k   */
        seq_t val = big_pow((seq_t)2, exp);  /* 2^(2^k) */
        return val - (seq_t)1;
    }
}
