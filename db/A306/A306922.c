/*
 * A306922  Number of distinct powers of two obtained by breaking the binary
 *          representation of n into consecutive blocks, and then adding the
 *          numbers represented by the blocks.
 * https://oeis.org/A306922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A306922
 * Formula: a(n) = number of distinct powers of two obtained by
 * partitioning the binary representation of n into consecutive
 * blocks, summing each block, and checking if the sum is a power of two.
 * Offset: 1
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for 1‑based index n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */

    /* Determine bit length L of n. */
    idx_t tmp = n;
    int L = 0;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }
    if (L == 0) return 0;            /* n == 0, should not happen */

    /* Store the bits of n in MSB‑first order. */
    uint8_t bits[64] = {0};
    for (int i = 0; i < L; ++i) {
        bits[i] = (uint8_t)((n >> (L - 1 - i)) & 1);
    }

    /* Number of possible partitions: 2^(L-1), or 1 if L == 1. */
    idx_t partitions = (L <= 1) ? (idx_t)1
                                : ((idx_t)1 << (L - 1));

    /* Bit mask indicating which powers of two have already been seen.
       At most L different powers can occur (2^(0) … 2^(L-1)). */
    uint64_t seen = 0;          /* each bit k corresponds to 2^k */

    for (idx_t mask = 0; mask < partitions; ++mask) {
        idx_t sum   = 0;
        idx_t block = 0;

        for (int i = 0; i < L; ++i) {
            block = (block << 1) | bits[i];

            /* Cut after this bit iff the corresponding mask bit is set. */
            if (i < L - 1 && ((mask >> i) & 1U)) {
                sum += block;
                block = 0;
            }
        }
        sum += block;           /* add the final block */

        /* Test whether sum is a power of two. */
        if (sum != 0 && (sum & (sum - 1)) == 0) {
            /* Determine k such that sum == 1 << k. */
            idx_t k = 0;
            idx_t s = sum;
            while (s >>= 1) ++k;          /* shift until zero */
            if (k < 64) seen |= ((uint64_t)1 << k);
        }
    }

    /* Count the distinct powers of two that were found. */
    seq_t cnt = 0;
    uint64_t m = seen;
    while (m) {
        cnt += (seq_t)(m & 1);
        m >>= 1;
    }
    return cnt;
}
