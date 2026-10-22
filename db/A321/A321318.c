/*
 * A321318  Number of distinct values obtained by partitioning the binary
 *          representation of n into consecutive blocks, and then summing the
 *          numbers represented by the blocks.
 * https://oeis.org/A321318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = number of distinct sums obtained by partitionering the
 *            binary representation of n into consecutive blocks and summing
 *            the values of the blocks
 * Range:     u8 n <= 2026, u16 n <= 9361, u32 n <= 9361, u64 n <= 9361,
 *            u128 n <= 9361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are computed by brute‑force enumeration of all 2^(L-1) partitions, where L is the bit length of n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: compute number of bits needed to represent n (n>=1). */
static inline int bitlen_idx(idx_t n)
{
    int l = 0;
    while (n != 0) {
        l++;
        n >>= 1;
    }
    return l > 0 ? l : 1;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* not used, but harmless */

    /* Determine bit length L of n. */
    int L = bitlen_idx(n);
    if (L == 0)
        L = 1;

    /* Allocate array of bits (MSB first). */
    uint8_t bits[64];
    int i;
    for (i = 0; i < L; ++i) {
        /* bit at position i (from MSB) */
        bits[i] = (uint8_t)((n >> (L - 1 - i)) & 1ULL);
    }

    /* Visits array: sums are bounded by n, so allocate n+1 booleans. */
    size_t maxSum = (size_t)n;
    bool *seen = (bool *)malloc((maxSum + 1) * sizeof(bool));
    if (!seen)
        return 0;           /* fallback on allocation failure */

    for (size_t j = 0; j <= maxSum; ++j)
        seen[j] = false;

    seq_t distinct = 0;

    /* Number of possible partitions: 2^(L-1) (one choice per possible cut). */
    size_t totalMasks = (L == 1) ? 1 : ((size_t)1 << (L - 1));

    for (size_t mask = 0; mask < totalMasks; ++mask) {
        seq_t sum = 0;
        seq_t block = 0;

        for (int pos = 0; pos < L; ++pos) {
            block = (block << 1) | bits[pos];
            if (pos < L - 1 && (mask & ((size_t)1 << pos))) {
                sum += block;
                block = 0;
            }
        }
        sum += block;          /* last block */

        /* Record distinct sums. */
        if (!seen[(size_t)sum]) {
            seen[(size_t)sum] = true;
            distinct++;
        }
    }

    free(seen);
    return distinct;
}
