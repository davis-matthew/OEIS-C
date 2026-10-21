/*
 * A295890  a(n) = 1 if binary weights of n and 3n have different parity, 0
 *          otherwise; a(n) = A010060(n) XOR A010060(3n).
 * https://oeis.org/A295890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A000120(n) + A000120(3*n)) mod 2
 * Range:     u8 n <= 9190, u16 n <= 9190, u32 n <= 9190, u64 n <= 9190,
 *            u128 n <= 9190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* popcount of n (uint64_t popcount works uniformly) */
    uint64_t n0 = (uint64_t)n;
    int pc_n = __builtin_popcountll(n0);

    /* popcount of 3*n – computed in 128 bits and split into two 64‑bit parts */
    unsigned __int128 t = (unsigned __int128)n0 * 3;
    uint64_t lo = (uint64_t)t;
    uint64_t hi = (uint64_t)(t >> 64);
    int pc_3n = __builtin_popcountll(lo) + __builtin_popcountll(hi);

    /* parity of the sum of popcounts */
    int res = (pc_n + pc_3n) & 1;
    return (seq_t)res;
}
