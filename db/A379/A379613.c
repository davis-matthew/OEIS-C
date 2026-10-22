/*
 * A379613  a(n) = n^(n - 1) - 2*(n + 1)^(n - 2), by convention a(0) = 0.
 * https://oeis.org/A379613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) - 2*(n+1)^(n-2) for n >= 3; a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    seq_t t1 = seq_pow((seq_t)n, n - 1);
    seq_t t2 = seq_pow((seq_t)(n + 1), n - 2);
    return t1 - (t2 + t2);
}
