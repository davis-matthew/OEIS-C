/*
 * A352993  a(n) is the n-th positive integer that has no common 1-bit with n;
 *          a(0) = 0.
 * https://oeis.org/A352993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352993: a(n) is the n-th positive integer that has no
   common 1-bit with n (a(0)=0).
   Formula: For n > 0, let
       z0 < z1 < z2 < ... be the positions of zeros in the binary
       expansion of n.
   Then
       a(n) = Σ  ( bit i of n ) * 2^z_i   (sum over i = 0 .. ⌊log₂ n⌋)
   (the bit i of n acts as the i‑th bit of the index t = n used in the
     bijection between the set of numbers disjoint from n and binary
     numbers).
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Binary length of n (number of bits needed to represent n) */
    idx_t bitlen = 64 - __builtin_clzll(n);          /* n > 0 */

    /* Positions of zeros in the binary expansion of n.
       We need exactly `bitlen` such positions.   */
    int zero_pos[64];
    idx_t zeros_needed = bitlen;
    idx_t p = 0, zeros_found = 0;
    while (zeros_found < zeros_needed) {
        int bit = (p < 64) ? (int)((n >> p) & 1ULL) : 0;
        if (bit == 0) {
            zero_pos[zeros_found++] = (int)p;
        }
        ++p;
    }

    /* Build the result: for each set bit i of n use zero_pos[i] */
    seq_t res = 0;
    for (idx_t i = 0; i < bitlen; ++i) {
        if ((n >> i) & 1ULL) {
            res += ((seq_t)1 << (unsigned)zero_pos[(int)i]);
        }
    }

    return res;
}
