/*
 * A359162  a(n) = 1 if n is a number of the form 4u+3 with an even number of
 *          prime factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A359162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 3 (mod 4) and bigomega(n) is even, else 0
 * Range:     u8 n <= 9754, u16 n <= 9754, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n ≥ 1 */
    if ((n & 3) != 3) return (seq_t)0;   /* n % 4 must be 3 */

    seq_t omega = bigomega(n);           /* total number of prime factors (with multiplicity) */
    return ((omega % 2) == 0) ? (seq_t)1 : (seq_t)0;
}
