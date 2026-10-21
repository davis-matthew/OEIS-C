/*
 * A249861  a(n) are values of k that yield "record-breaking" integer sequence
 *          lengths for the recursion: b(i) = 3*(b(i-1) + b(i-2))/2, with b(0)
 *          = 1 and b(1) = k.
 * https://oeis.org/A249861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=1; for n>1 a(n)=a(n-1)+2^(Nmax(a(n-1))‑1) where Nmax(k) is
 *            the largest i such that the sequence b(0)=1, b(1)=k,
 *            b(i)=3·(b(i‑1)+b(i‑2))/2 yields an integer. The sequence a(n)
 *            gives the k‑values that produce record‑breaking integer lengths
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static idx_t Nmax_k(seq_t k)
{
    seq_t b0 = (seq_t)1;
    seq_t b1 = k;
    idx_t i  = 1;   /* last index with integer value (b0,b1) */

    while (true) {
        seq_t sum = b0 + b1;
        /* check parity: integer continuation only if sum is even */
        if ((sum & (seq_t)1) == (seq_t)0) {
            seq_t half = sum >> (idx_t)1;          /* sum/2 */
            seq_t next = half * (seq_t)3;          /* 3*sum/2 */
            b0 = b1;
            b1 = next;
            ++i;
        } else {
            break;
        }
    }
    return i;   /* last integer index; Nmax = i */
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;        /* undefined for n<1, return 0 */

    seq_t val = (seq_t)1;      /* a(1) = 1 */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        idx_t nm  = Nmax_k(val);     /* longest integer index for current k */
        seq_t inc = pow2(nm - (idx_t)1); /* 2^(nm-1) */
        val = val + inc;
    }

    return val;
}
