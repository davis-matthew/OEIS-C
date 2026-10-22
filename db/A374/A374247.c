/*
 * A374247  The greatest number of runs possible in a permutation of the prime
 *          factors of n (A373957) minus the number of distinct such factors
 *          (A001221).
 * https://oeis.org/A374247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A373957(n) - A001221(n) where A373957(n) is the maximum
 *            number of runs in a permutation of the multiset of prime factors
 *            of n, and A001221(n) is the number of distinct prime factors of
 *            n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   seq_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed on the fly by factorising n with
   simple trial division.  The algorithm runs in O(sqrt(n)) time,
   which is easily fast enough for n up to about 10 000. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_a.h"

/* The required external linkage function */
seq_t a(idx_t n)
{
    return seq_a(n);
}
