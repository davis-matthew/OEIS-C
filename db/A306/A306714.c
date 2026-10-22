/*
 * A306714  Permanent of the circulant matrix whose first row is given by the
 *          binary expansion of n.
 * https://oeis.org/A306714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Permanent of the circulant matrix whose first row is the binary
 *            expansion of n
 * Range:     u8 n <= 46, u16 n <= 382, u32 n <= 8190, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* number of bits in n */
    idx_t k = 64 - __builtin_clzll(n);
    size_t size = 1u << k;
    idx_t maskAll = (k == 64) ? ~0ULL : ((1ULL << k) - 1);
    idx_t rmask = n & maskAll;              /* first row mask, column 0 is the most significant bit */

    seq_t *dp   = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(size, sizeof(seq_t));
    if (!dp || !next) return 0;             /* memory failure, give up safely */

    dp[0] = 1;
    for (idx_t row = 0; row < k; ++row) {
        /* columns allowed for this row: rotate right by 'row' */
        idx_t shift  = row;
        idx_t lower  = rmask & ((1ULL << shift) - 1);
        idx_t allowed = ((rmask >> shift) | (lower << (k - shift))) & maskAll;

        /* clear next */
        for (size_t i = 0; i < size; ++i) next[i] = 0;

        for (size_t mask = 0; mask < size; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;

            idx_t freeMask = (~mask) & maskAll;
            idx_t avail = allowed & freeMask;

            while (avail) {
                idx_t bit = avail & -avail;          /* lowest set column */
                next[mask | bit] += ways;
                avail ^= bit;
            }
        }
        /* swap dp and next for next iteration */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[size - 1];
    free(dp);
    free(next);
    return result;
}
