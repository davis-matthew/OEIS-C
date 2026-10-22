/*
 * A331216  a(n) is the number of ways to write n = u + v where the binary
 *          representations of u and of v have the same number of 0's and the
 *          same number of 1's.
 * https://oeis.org/A331216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ordered pairs (u, v) such that * u + v = n, the
 *            binary representations of u and v have the * same number of 1's
 *            and the same number of 0's (leading zeros * are ignored)
 * Range:     u8 n <= 5409, u16 n <= 9527, u32 n <= 9527, u64 n <= 9527,
 *            u128 n <= 9527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: bit length of a uint64_t value.
 * For 0 the length is defined as 0. */
static inline int bitlen_u64(idx_t x)
{
    return x ? 64 - __builtin_clzll((unsigned long long)x) : 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t u = 0; u <= n; ++u)
    {
        idx_t v = n - u;
        if (bitlen_u64(u) == bitlen_u64(v) &&
            __builtin_popcountll(u) == __builtin_popcountll(v))
        {
            ++count;
        }
    }
    return count;
}
