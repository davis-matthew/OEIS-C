/*
 * A373180  a(n) = min{k : KroneckerSymbol(n, k) = 1} if n > 1 and k > 1, a(0)
 *          = 1, a(1) = 0.
 * https://oeis.org/A373180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k>1 : KroneckerSymbol(n,k)=1} for n>1, with a(0)=1,
 *            a(1)=0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   kronecker_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/kronecker_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t k = 2;

    for (;;) {
        if (kronecker_uint64(N, k) == 1) {
            return (seq_t)k;
        }
        ++k;
    }
}
