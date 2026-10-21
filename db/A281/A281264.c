/*
 * A281264  Base-2 logarithm of denominator of (Sum_{k=0..n^2-1}
 *          (-1)^k*sqrt(Pi)/(Gamma(1/2-k)*Gamma(1+k))) - n.
 * https://oeis.org/A281264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 - 2 - popcount(n^2 - 1) (popcount = number of
 *            1-bits)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <limits.h>

static inline uint64_t popcount_seq_t(seq_t x)
{
    /* Count the 1-bits in an arbitrary-width integer type seq_t. */
    uint64_t count = 0;
    /* Number of bits in seq_t */
    const size_t total_bits = sizeof(seq_t) * CHAR_BIT;
    /* Number of 64‑bit chunks needed */
    const size_t chunks = (total_bits + 63) / 64;
    for (size_t i = 0; i < chunks; ++i) {
        /* Shift amount is guaranteed < width of seq_t */
        uint64_t chunk = (uint64_t)(x >> (i * 64));
        count += __builtin_popcountll(chunk);
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;            /* cast index to seq_t */
    seq_t sq = sn * sn;             /* n^2 */
    seq_t m  = sq - (seq_t)1;       /* n^2 - 1 */
    uint64_t ones = popcount_seq_t(m);
    seq_t result = (seq_t)2 * sq - (seq_t)2 - (seq_t)ones;
    return result;
}
