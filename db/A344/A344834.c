/*
 * A344834  Square array T(n, k), n, k >= 0, read by antidiagonals; T(n, k) =
 *          (n * 2^max(0, w(k)-w(n))) AND (k * 2^max(0, w(n)-w(k))) (where AND
 *          denotes the bitwise AND operator and w = A070939).
 * https://oeis.org/A344834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9203, u16 n <= 9203, u32 n <= 9203, u64 n <= 9203,
 *            u128 n <= 9203, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A344834: a(n) = value of T(n,k) where (n,k) are the coordinates of the
 * pair read by antidiagonals of the square array T.  For (n,k):
 *   T(n,k) = (n << max(0,bitlen(k)-bitlen(n)))  &
 *            (k << max(0,bitlen(n)-bitlen(k)))
 * (bitlen(x) is the number of binary digits of x, defined by
 *  unsigned bitlen_seq_t(seq_t x);)
 */

#include "../utils/bitlen_seq_t.h"   /* provides unsigned bitlen_seq_t(seq_t) */

static seq_t T_val(seq_t x, seq_t y)
{
    if (x == 0 || y == 0) return 0;

    unsigned lx = bitlen_seq_t(x);
    unsigned ly = bitlen_seq_t(y);

    if (lx > ly) {
        return x & (y << (lx - ly));
    } else if (ly > lx) {
        return (x << (ly - lx)) & y;
    } else {
        return x & y;
    }
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that S(d-1) <= n < S(d),
       where S(d) = (d+1)*(d+2)/2 is the cumulative count of terms
       up to and including diagonal d. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    idx_t prevS = d * (d + 1) / 2;   /* S(d-1) */
    idx_t r    = n - prevS;          /* offset within diagonal d */

    idx_t ncoord = r;                /* first coordinate */
    idx_t kcoord = d - r;            /* second coordinate */

    return T_val((seq_t)ncoord, (seq_t)kcoord);
}
