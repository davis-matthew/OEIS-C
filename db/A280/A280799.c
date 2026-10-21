/*
 * A280799  a(n) = A049502(phi(n)).
 * https://oeis.org/A280799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of positions of 1→0 transitions in the binary
 *            expansion of φ(n) (positions counted from 1 at the
 *            least‑significant bit)
 * Range:     u8 n <= 1318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t tot = phi(n);
    seq_t sum = 0;
    idx_t pos = 1;
    seq_t x = tot;
    while (x > 1)
    {
        seq_t cur  = x & 1;
        seq_t next = (x >> 1) & 1;
        if (cur && !next)
            sum += (seq_t)pos;
        x >>= 1;
        ++pos;
    }
    return sum;
}
