/*
 * A341952  Let x = (prime(n+1) - prime(n))/2 modulo 3 for n >= 2, then a(n) =
 *          -1 if x = 2, otherwise a(n) = x.
 * https://oeis.org/A341952
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((prime(n+1) - prime(n)) / 2) mod 3 ; a(n) = -1 if the
 *            residue equals 2, otherwise a(n) equals the residue
 * Range:     i8 n <= 106, i16 n <= 106, i32 n <= 106, i64 n <= 106,
 *            i128 n <= 106, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2: a(2) is the first term in the list.  We assume n ≥ 2. */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Compute the gap and divide by 2 (integer division).  Then reduce modulo 3. */
    uint32_t gap   = p2 - p1;
    uint32_t resid = (gap / 2) % 3;

    return (resid == 2) ? (seq_t)(-1) : (seq_t)resid;
}
