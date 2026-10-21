/*
 * A267445  Number of ON (black) cells in the n-th iteration of the "Rule 129"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(0, 2*n - 2^(popcount(n)+1) + 1), a(0) = 1
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount(n) is the number of 1-bits in the binary expansion of n. */
#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Count the bits set in n.  idx_t is a 64-bit unsigned type. */
    unsigned int pc = __builtin_popcountll((unsigned long long)n);
    unsigned int shift = pc + 1;                 /* exponent of 2 */

    /* Use a signed __int128 for the intermediate computation to avoid
       underflow or overflow of unsigned types.  This type is available
       in both C11 and C++17. */
    __int128 pow2 = (__int128)1 << shift;        /* 2^(pc+1)  */
    __int128 val   = (__int128)2 * n - pow2 + 1; /* 2*n - 2^(pc+1) + 1 */

    if (val <= 0)
        return (seq_t)0;
    return (seq_t)val;
}
