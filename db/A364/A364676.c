/*
 * A364676  Lower independence number of the n-cube connected cycle graph.
 * https://oeis.org/A364676
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*2^(n-2) for all n ≥ 3, except a(5)=47
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 5)
        return 47;
    return (seq_t)n * pow2(n - 2);
}
