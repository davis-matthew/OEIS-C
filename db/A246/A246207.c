/*
 * A246207  Permutation of nonnegative integers: a(0) = 0, a(1) = 1, a(2n) =
 *          A117968(a(n)), a(2n+1) = A117967(1+a(n)).
 * https://oeis.org/A246207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0 a(1) = 1 a(2n) = A117968(a(n)) a(2n+1) = A117967(1 +
 *            a(n)) A117968 and A117967 are the two complementary permutations
 *            obtained by mapping the balanced‑ternary digits of an integer:
 *            balance digit -1 → 1 , 0 → 0 , 1 → 2 (A117968) balance digit -1
 *            → 2 , 0 → 0 , 1 → 1 (A117967) The function...
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/perm.h"

/* Main sequence function.  idx_t is an unsigned integer type. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1;

    if ((n & (idx_t)1) == (idx_t)0)          /* even index */
        return perm(a(n >> 1), true);       /* A117968(a(n/2)) */
    else                                     /* odd index */
        return perm((seq_t)1 + a(n >> 1), false); /* A117967(1 + a((n-1)/2)) */
}
