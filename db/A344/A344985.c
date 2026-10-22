/*
 * A344985  Sum of imbalance of all initial subsequences of the binary
 *          representation of n.
 * https://oeis.org/A344985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 9930, u16 n <= 9930, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   absdiff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n>>1) + |#bits(n) - 2*popcount(n)| ,
   with a(1) = 1.  Here #bits(n) is the length of the binary
   representation of n (no leading zeros).  This matches
   OEIS A344985: the sum of imbalances of all initial binary
   subsequences of n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/absdiff.h"

static inline int bitlen_u64(idx_t v)
{
    int bits = 0;
    while (v) { bits++; v >>= 1; }
    return bits;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    while (n)
    {
        int ones   = __builtin_popcountll((unsigned long long)n);
        int total  = bitlen_u64(n);
        int imbalance = absdiff(total, 2 * ones); /* A037861(n) absolute value */
        result += (seq_t)imbalance;
        n >>= 1;
    }
    return result;
}
