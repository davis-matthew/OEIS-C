/*
 * A246831  a(n) is the concatenation of n and 3n in binary.
 * https://oeis.org/A246831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n | 3n)_2 (concatenation of n and 3n in binary)
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9161 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;       /* convert index to seq_t */
    seq_t t = n_seq * (seq_t)3;   /* compute 3n */
    unsigned bits = bit_length(t);   /* number of bits in 3n */
    return (n_seq << bits) | t;               /* concatenate */
}
