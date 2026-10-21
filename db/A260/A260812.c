/*
 * A260812  a(n) is the number of edges in a rooted 2-ary tree built from the
 *          binary representation of n: each vertex at level i
 *          (i=0,...,floor(log_2(n))) has two children if the i-th most
 *          significant bit is 1 and one child if the i-th bit is 0.
 * https://oeis.org/A260812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1, a(1) = 2, a(n) = a(n >> 1) + 2^popcount(n) for n >= 2
 *            (popcount counts the 1-bits in the binary representation of n)
 * Range:     u8 n <= 190, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* recursive part */
    seq_t sub = a(n >> 1);

    /* compute 2^popcount(n) safely */
    unsigned int pc = __builtin_popcountll((unsigned long long)n);
    seq_t pow2 = (seq_t)1;
    for (unsigned int i = 0; i < pc; ++i)
        pow2 <<= 1;   /* multiplying by 2 each iteration */

    return sub + pow2;
}
