/*
 * A335063  a(n) = Sum_{k=0..n} (binomial(n,k) mod 2) * k.
 * https://oeis.org/A335063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^(A000120(n)-1) where A000120(n) is popcount of n
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t pc = __builtin_popcountll((uint64_t)n);
    seq_t pow = 1;
    /* multiply by 2^{pc-1} */
    for (idx_t i = 0; i + 1 < pc; ++i)
        pow <<= 1;
    return (seq_t)n * pow;
}
