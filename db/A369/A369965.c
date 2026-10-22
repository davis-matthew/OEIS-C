/*
 * A369965  a(n) = 1 if n is of the form 4m+2 and it has an even number of
 *          prime factors with multiplicity, otherwise 0.
 * https://oeis.org/A369965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 2 (mod 4) and Ω(n) is even, otherwise 0
 * Range:     u8 n <= 9876, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if ((n & 3ULL) != 2ULL) return (seq_t)0;
    seq_t w = bigomega(n);
    unsigned long long uw = (unsigned long long)w;
    return (uw & 1ULL) ? (seq_t)0 : (seq_t)1;
}
