/*
 * A359372  a(n) = 1 if n is not a multiple of 4 and has an odd number of
 *          prime factors (with multiplicity), otherwise a(n) = 0.
 * https://oeis.org/A359372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n mod 4 ≠ 0 and Ω(n) (number of prime factors with
 *            multiplicity) is odd; otherwise 0
 * Range:     u8 n <= 9546, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; return 0 for n == 0 */
    if (n == 0) return 0;

    /* skip multiples of 4 */
    if ((n & 3) == 0) return 0;

    /* compute the parity of the total number of prime factors (with multiplicity) */
    seq_t ω = bigomega(n);        /* Ω(n) returns seq_t */
    return (ω & 1U) ? 1 : 0;      /* odd → 1, even → 0 */
}
