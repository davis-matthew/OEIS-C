/*
 * A384956  Binary XOR of number of 1-bits in the binary representation of n
 *          and number of 0-bits in the binary representation of n, a(0) = 1.
 * https://oeis.org/A384956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(n) = popcount(n) XOR (len(n) - popcount(n))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where len(n) is the number of bits in the binary representation of n */
#include <stdint.h>
#include <limits.h>

/* Count the number of 1-bits in a 64‑bit integer.
   Uses the GCC/Clang built‑in. On other compilers the call can be
   replaced by a portable popcount implementation. */
static inline int popcount_u64(idx_t v)
{
    return __builtin_popcountll((unsigned long long)v);
}

/* Compute floor(log2(v)) for v > 0. If v == 0, return -1 to indicate empty. */
static inline int floor_log2_uint64(idx_t v)
{
    /* __builtin_clzll undefined for v == 0, so caller must handle that case. */
    return 63 - __builtin_clzll((unsigned long long)v);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle n == 0 separately: its binary representation is a single 0 bit. */
    if (n == 0)
    {
        seq_t ones  = (seq_t)0;
        seq_t zeros = (seq_t)1;  /* len(0) = 1, ones = 0 */
        return ones ^ zeros;     /* 0 XOR 1 = 1 */
    }

    /* Count 1-bits */
    int ones_int   = popcount_u64(n);
    seq_t ones     = (seq_t)ones_int;

    /* Length of binary representation (no leading zero bits) */
    int len_int    = floor_log2_uint64(n) + 1;
    seq_t len      = (seq_t)len_int;

    /* Number of 0-bits is len - popcount */
    seq_t zeros    = len - ones;

    /* Return XOR of popcount and zero-count */
    return ones ^ zeros;
}
