/*
 * A358753  a(n) = 1 if bigomega(n) == 3 (mod 4), otherwise 0.
 * https://oeis.org/A358753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [bigomega(n) == 3 (mod 4)]
 * Range:     u8 n <= 9860, u16 n <= 9860, u32 n <= 9860, u64 n <= 9860,
 *            u128 n <= 9860, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* out of range, return 0 */
    seq_t w   = bigomega(n);              /* number of prime factors with multiplicity */
    seq_t mod = w % (seq_t)4;             /* w modulo 4 */
    return (mod == (seq_t)3) ? (seq_t)1 : (seq_t)0;
}
