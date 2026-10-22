/*
 * A301270  Number of labeled trees on n vertices containing two fixed
 *          non-adjacent edges.
 * https://oeis.org/A301270
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * n^(n-4) (for n ≥ 4, otherwise 0)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t pw = big_pow((seq_t)n, (seq_t)(n - 4));
    return pw * ((seq_t)4);
}
