/*
 * A359770  a(n) = 1 if n and bigomega(n) are of different parity, otherwise
 *          0. Here bigomega (A001222) gives the number of prime factors of n
 *          with multiplicity.
 * https://oeis.org/A359770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n and bigomega(n) have different parity, else 0.
 *            a(n) = ((n mod 2) XOR (bigomega(n) mod 2))
 * Range:     u8 n <= 9943, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* bigomega is a helper that returns the number of prime factors with multiplicity.
       We only need its least significant bit to determine parity. */
    return (seq_t)((n & 1) ^ (bigomega(n) & 1));
}
