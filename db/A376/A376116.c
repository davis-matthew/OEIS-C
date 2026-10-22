/*
 * A376116  Number of times the root fires in a chip-firing game starting with
 *          2n chips placed at the root on an infinite binary tree with a loop
 *          at the root.
 * https://oeis.org/A376116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j = 1}^{m-1} (2^j - 1) * (b_j + 1), where m =
 *            floor(log2(2n+1)) and b_j is the j‑th bit of 2n+1 (bit 0 is the
 *            least significant bit)
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2n + 1 fits in 64 bits for all valid n */
    uint64_t val = ((uint64_t)n << 1) + 1ULL;   /* val = 2n+1 */

    /* highest set bit index m (0‑based) */
    idx_t m = 63 - __builtin_clzll(val);        /* 0 <= m <= 63 */

    seq_t sum = 0;
    for (idx_t j = 1; j < m; ++j) {
        uint64_t bit = (val >> j) & 1ULL;        /* bit j of val */
        seq_t term = ((seq_t)1 << j) - 1;       /* 2^j - 1 */
        term *= (seq_t)(bit + 1);                /* multiply by (bit + 1) */
        sum += term;
    }
    return sum;
}
