/*
 * A351047  a(n) = Sum_{k=1..n} n^rad(k).
 * https://oeis.org/A351047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} n^rad(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by summing n raised to the radical of each k up to n. */

#include "../utils/rad.h"
#include "../utils/big_pow.h"
#include <stdint.h>   /* for idx_t, seq_t types via build system */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t pw = big_pow((seq_t)n, rad(k));
        sum += pw;
    }
    return sum;
}
