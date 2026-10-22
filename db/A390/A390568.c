/*
 * A390568  The number k such that the k-th composition in standard order is
 *          the first sums of the n-th composition in standard order.
 * https://oeis.org/A390568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index_of(first_sums(composition_of(n)))
 * Range:     u8 n <= 40, u16 n <= 640, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find positions of 1 bits in the binary representation of n.
       Positions are counted from 1 (LSB has position 1). */
    unsigned int pos[64];
    unsigned int len = 0;
    unsigned long long tmp = (unsigned long long)n;
    for (unsigned int i = 0; tmp; ++i) {
        if (tmp & 1ULL)
            pos[len++] = i + 1;
        tmp >>= 1;
    }

    /* Composition of n has length equal to the number of 1‑bits.
       If the composition consists of at most one part,
       its first sums are empty, so a(n)=0. */
    if (len <= 1)
        return (seq_t)0;

    /* Build the composition from the gaps between consecutive
       1‑bit positions, but placed in reverse order. */
    unsigned int comp[64];
    for (unsigned int i = 0; i < len; ++i) {
        unsigned int diff = pos[i] - (i ? pos[i-1] : 0);
        comp[len - 1 - i] = diff;
    }

    /* First sums of the composition.
       New composition has length len-1. */
    unsigned int newlen = len - 1;
    unsigned int newcomp[64];
    for (unsigned int i = 0; i < newlen; ++i)
        newcomp[i] = comp[i] + comp[i+1];

    /* Convert the first‑sums composition back to its index.
       The index is obtained by interpreting the reversed
       differences as gaps between 1‑bit positions. */
    unsigned int curPos = 0;
    seq_t result = 0;
    for (int i = newlen - 1; i >= 0; --i) {
        curPos += newcomp[i];
        seq_t term = (seq_t)1 << (curPos - 1);
        result += term;
    }

    return result;
}
