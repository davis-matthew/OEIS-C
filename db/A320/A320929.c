/*
 * A320929  Positions of 1 in A320927.
 * https://oeis.org/A320929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A320927 (A320927 = 0, then
 *            segments of the morphism 0->001, 1->010 applied successively).
 *            Segment k has 3^(k-1) ones, one per parent term; the child of
 *            parent j sits at 3j+2 if the parent is 0, else 3j+1
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 926 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A320929_parent_val(idx_t j, int levels)
{
    int d[64];
    int i;
    int v = 0;
    for (i = 0; i < levels; i++) {
        d[i] = (int)(j % 3);
        j /= 3;
    }
    for (i = levels - 1; i >= 0; i--) {
        if (v == 0) v = (d[i] == 2) ? 1 : 0;
        else        v = (d[i] == 1) ? 1 : 0;
    }
    return v;
}

seq_t a(idx_t n)
{
    idx_t ones = 1;        /* ones in segment k (3^(k-1)) */
    idx_t seglen = 3;      /* length of segment k */
    idx_t len_before = 1;  /* total length of segments 0..k-1 */
    int k = 1;
    while (n > ones) {
        n -= ones;
        len_before += seglen;
        seglen *= 3;
        ones *= 3;
        k++;
    }
    idx_t j = n - 1;
    int pv = A320929_parent_val(j, k - 1);
    idx_t off = (pv == 0) ? 2 : 1;
    return (seq_t)len_before + (seq_t)j * 3 + (seq_t)off + 1;
}
