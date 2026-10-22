/*
 * A380008  Numbers t whose binary expansion Sum 2^e_i has exponents e_i which
 *          are odious numbers (A000069).
 * https://oeis.org/A380008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maps the bits of n to odious positions (odd-popcount exponents).
   For each set bit i of the binary representation of n:
      - If popcount(i) is odd, put a 1 at exponent 2*i.
      - Else put a 1 at exponent 2*i+1.
   The resulting value is a(n). */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t i = 0;
    while (n) {
        if (n & 1U) {
            /* Popcount parity of i decides which exponent is odious. */
            if (__builtin_popcountll(i) & 1U) {
                res += ((seq_t)1 << (unsigned)(2 * i));
            } else {
                res += ((seq_t)1 << (unsigned)(2 * i + 1));
            }
        }
        n >>= 1;
        ++i;
    }
    return res;
}
