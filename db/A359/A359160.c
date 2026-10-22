/*
 * A359160  a(n) = 1 if n is a number of the form 4u+1 with an even number of
 *          prime factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A359160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 1 (mod 4) and Ω(n) (count of prime factors with
 *            multiplicity) is even, otherwise 0
 * Range:     u8 n <= 9645, u16 n <= 9645, u32 n <= 9645, u64 n <= 9645,
 *            u128 n <= 9645, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n) {
    /* n ≥ 1; behavior for n = 0 is irrelevant */
    if ((n & 3) != 1) return 0;          /* not of the form 4u+1 */
    seq_t om = bigomega(n);             /* total prime factors with multiplicity */
    return ((om % (seq_t)2) == 0) ? (seq_t)1 : (seq_t)0;
}
