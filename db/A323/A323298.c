/*
 * A323298  Number of 3-uniform hypergraphs spanning n labeled vertices where
 *          every two edges have exactly one vertex in common.
 * https://oeis.org/A323298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=a(2)=a(4)=a(8)=0, a(3)=1, a(5)=15, a(6)=150,
 *            a(7)=1815. For even n>6: a(n)=0. For odd n≥9: a(n)=n · (n‑2)!!
 *            (double factorial of the odd numbers ≤n‑2). The non‑star
 *            hypergraphs for n=6 and n=7 are counted in the special values
 *            above; all other cases have a single common vertex in eve...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 34, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the fixed small values */
    switch (n) {
    case 0:  return (seq_t)1;
    case 1:  /* fall through */
    case 2:
    case 4:  return (seq_t)0;
    case 3:  return (seq_t)1;
    case 5:  return (seq_t)15;
    case 6:  return (seq_t)150;
    case 7:  return (seq_t)1815;
    case 8:  return (seq_t)0;
    }

    /* even n larger than 6 -> no hypergraph */
    if ((n & 1) == 0) {
        return (seq_t)0;
    }

    /* odd n ≥ 9:  a(n) = n · (n‑2)!!  = n · Π_{k=1}^{(n-3)/2} (2k-1) */
    seq_t res = (seq_t)n;
    for (idx_t j = 1; j <= n - 2; j += 2) {
        res *= (seq_t)j;
    }
    return res;
}
