/*
 * A359172  a(n) = 1 if n is not a multiple of 3 and has an odd number of
 *          prime factors (with multiplicity), otherwise a(n) = 0.
 * https://oeis.org/A359172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not a multiple of 3 and Ω(n) is odd, otherwise
 *            0
 * Range:     u8 n <= 9140, u16 n <= 9140, u32 n <= 9140, u64 n <= 9140,
 *            u128 n <= 9140, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; return 0 for n < 1 as a harmless guard. */
    if (n < 1) return (seq_t)0;

    /* If n is a multiple of 3, the value is 0. */
    if ((n % 3) == 0) return (seq_t)0;

    /* bigomega(n) gives the count of prime factors of n (with multiplicity). */
    seq_t omega = bigomega(n);

    /* Return 1 if ω(n) is odd, else 0. */
    return (seq_t)((omega & 1) ? 1 : 0);
}
