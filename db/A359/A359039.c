/*
 * A359039  Number of Wachs permutations of size n.
 * https://oeis.org/A359039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n = 2m: a(n) = m! * 2^m if n = 2m+1: a(n) = (m+1)! *
 *            2^m
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 32, u128 n <= 56,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = n >> 1;         /* m = floor(n/2) */
    if (n & 1) {              /* n is odd: n = 2m+1 */
        return factorial(m + 1) * pow2(m);
    } else {                  /* n is even: n = 2m */
        return factorial(m) * pow2(m);
    }
}
