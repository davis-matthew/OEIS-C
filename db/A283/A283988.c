/*
 * A283988  a(n) = A002487(n-1) AND A002487(n), where AND is bitwise-and
 *          (A004198).
 * https://oeis.org/A283988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002487(n-1) AND A002487(n) where A002487 is Stern's
 *            diatomic sequence: A002487(0)=0, A002487(1)=1, and for n>1,
 *            A002487(2k)=A002487(k), A002487(2k+1)=A002487(k)+A002487(k+1)
 * Range:     u8 n <= 9381, u16 n <= 9835, u32 n <= 9835, u64 n <= 9835,
 *            u128 n <= 9835, big any n
 * Helpers:   stern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/stern.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined below offset, safe return */
    seq_t x = stern(n - 1);
    seq_t y = stern(n);
    return x & y;
}
