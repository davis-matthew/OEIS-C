/*
 * A338726  a(n) = Catalan(n) + 2^n - n - 1.
 * https://oeis.org/A338726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) + 2^n - n - 1 Offset: 0 (a(0) is the first
 *            term)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute Catalan(n) */
    seq_t res = catalan(n);

    /* Add 2^n */
    res += pow2(n);

    /* Subtract n + 1 */
    res -= (seq_t)n;
    res -= 1;

    return res;
}
