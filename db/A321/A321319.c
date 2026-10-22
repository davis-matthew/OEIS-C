/*
 * A321319  Smallest power of 2 obtainable by partitioning the binary
 *          representation of n into consecutive blocks and then summing.
 * https://oeis.org/A321319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   smallest power of two obtainable by partitioning the binary
 *            representation of n into consecutive blocks and summing. The
 *            function enumerates all 2^(L-1) possible cuts for an L‑bit
 *            number (n <= 10000 => L <= 14, so < 8192 partitions). For each
 *            partition it computes the sum of the block values and...
 * Range:     u8 n <= 5626, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: number of bits needed to represent n (n > 0) */
static inline idx_t bitlen(idx_t n)
{
    idx_t l = 0;
    while (n)
    {
        l++;
        n >>= 1;
    }
    return l;
}

/* helper: test whether x is a power of two (x > 0) */
static inline bool is_power_of_two(seq_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined in OEIS, but safe */

    idx_t L = bitlen(n);
    seq_t best = 0;                 /* 0 means none found yet */

    /* mask has (L-1) bits for cuts between consecutive bits */
    idx_t maxMask = (L > 1 ? (1ULL << (L - 1)) - 1 : 0);

    for (idx_t mask = 0; mask <= maxMask; ++mask)
    {
        seq_t sum = 0;
        seq_t cur = 0;

        /* iterate bits from most significant to least */
        for (idx_t i = 0; i < L; ++i)
        {
            idx_t bitPos = L - 1 - i;
            seq_t bit = (n >> bitPos) & 1;
            cur = (cur << 1) | bit;

            if (i + 1 < L)             /* there is a cut position after this bit */
            {
                idx_t cutPos = i;      /* bit position in mask */
                if ((mask >> cutPos) & 1ULL)
                {
                    sum += cur;
                    cur = 0;
                }
            }
        }
        sum += cur;                    /* last block */

        if (is_power_of_two(sum))
        {
            if (best == 0 || sum < best)
                best = sum;
        }
    }
    return best;
}
