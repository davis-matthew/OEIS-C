/*
 * A317613  Permutation of the nonnegative integers: lodumo_4 of A047247.
 * https://oeis.org/A317613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A317613: Permutation of the nonnegative integers.
   Apply the substitution to the least significant octal digit:
   0↔2, 1↔3, 4,5,6,7 remain unchanged.
   Formula: a(n) = (n - (n mod 8)) + M[n mod 8],
   where M = {2,3,0,1,4,5,6,7}.
   The sequence is a self‑inverse permutation: a(a(n)) = n.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n modulo 8 (fast by masking, as n is unsigned) */
    unsigned d = (unsigned)(n & 7u);
    /* mapping array for the right‑most octal digit */
    static const unsigned map[8] = {2,3,0,1,4,5,6,7};
    unsigned nd = map[d];
    /* replace the least significant octal digit */
    return (seq_t)(n - d + nd);
}
