/*
 * A293688  Partial sums of A002251.
 * https://oeis.org/A293688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*G(n) where G(0)=0 and * G(n) = n - G(G(n-1)) for
 *            n>0
 * Range:     u8 n <= 20, u16 n <= 325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_G
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, exit */
#include "../utils/get_G.h"

seq_t a(idx_t n)
{
    seq_t g = get_G(n);
    return ((seq_t)(n + 1)) * g;
}
