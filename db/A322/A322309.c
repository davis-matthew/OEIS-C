/*
 * A322309  Largest automorphism group size for a binary self-dual code of
 *          length 2n.
 * https://oeis.org/A322309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n! for all n except n = 4, where a(4) = 1344
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 4) return (seq_t)1344;
    return factorial(n) * pow2(n);
}
