/*
 * A280710  Characteristic function of squarefree semiprimes.
 * https://oeis.org/A280710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a squarefree semiprime (product of two distinct
 *            primes), 0 otherwise
 * Range:     u8 n <= 9856, u16 n <= 9856, u32 n <= 9856, u64 n <= 9856,
 *            u128 n <= 9856, big any n
 * Helpers:   bigomega, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t om = bigomega(n);
    if (om != (seq_t)2) return (seq_t)0;
    seq_t mu = mobius(n);
    return mu != (seq_t)0 ? (seq_t)1 : (seq_t)0;
}
