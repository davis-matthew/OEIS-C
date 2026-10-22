/*
 * A360737  Analog of the Moser-Newman sum sequence A005599, but counting 0's
 *          (instead of 1's) in the binary representation of 3*n.
 * https://oeis.org/A360737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (corrected):  a(n) = Σ_{i=0}^{n-1} (-1)^{bitlen(3*i)+popcount(3*i)}
 *  where bitlen(x) is the number of bits needed to represent x (0 for x=0),
 *  and popcount(x) is the number of 1‑bits in the binary representation of x.
 *  This yields the OEIS sequence A360737. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t x = 3 * i;
        unsigned int pop = __builtin_popcountll((unsigned long long)x);
        unsigned int bitlen;
        if (x == 0)
            bitlen = 0;
        else
            bitlen = 64 - __builtin_clzll((unsigned long long)x);
        /* parity of pop + bitlen equals parity of pop XOR bitlen */
        if ((pop ^ bitlen) & 1)
            --sum;
        else
            ++sum;
    }
    return sum;
}
