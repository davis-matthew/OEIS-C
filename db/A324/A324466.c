/*
 * A324466  Exponent of highest power of 2 that divides
 *          multinomial(3*n;n,n,n).
 * https://oeis.org/A324466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 3*A000120(n) - A000120(3*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000120 is the binary weight (popcount) of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* popcount of n (fits in 64 bits). */
    unsigned int pc_n = __builtin_popcountll((uint64_t)n);

    /* compute 3*n in at least 128 bits and popcount it. */
    unsigned __int128 three_n = (unsigned __int128)n * 3;
    uint64_t lo = (uint64_t)three_n;
    uint64_t hi = (uint64_t)(three_n >> 64);
    unsigned int pc_3n = __builtin_popcountll(lo) + __builtin_popcountll(hi);

    /* exponent of the highest power of 2 dividing the multinomial. */
    int64_t result = (int64_t)3 * pc_n - (int64_t)pc_3n;
    return (seq_t)result;
}
