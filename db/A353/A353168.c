/*
 * A353168  Polynomials over GF(2) that are divisible by x^2+x+1, encoded as
 *          binary numbers.
 * https://oeis.org/A353168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonnegative integer whose binary representation *
 *            has equal parity of bits in each residue class modulo 3 *
 *            (equivalently, the polynomial over GF(2) represented by the
 *            binary digits * is divisible by x²+x+1)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t v = 0;
    for (;;) {
        /* parity of bits in positions 0,1,2 (mod 3) */
        int p0 = 0, p1 = 0, p2 = 0;
        idx_t t = v;
        unsigned bit = 0;
        while (t) {
            if (t & 1ULL) {
                switch (bit % 3) {
                    case 0: p0 ^= 1; break;
                    case 1: p1 ^= 1; break;
                    case 2: p2 ^= 1; break;
                }
            }
            t >>= 1;
            ++bit;
        }
        /* all parities equal? */
        if ((p0 ^ p1) == 0 && (p1 ^ p2) == 0) {
            ++count;
            if (count == n)
                return (seq_t)v;   /* cast to the integer type used by the build */
        }
        ++v;
    }
}
