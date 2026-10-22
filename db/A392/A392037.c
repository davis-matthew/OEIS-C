/*
 * A392037  Square table read by downward antidiagonals: n-th row has e.g.f.
 *          (1-9*x)^(-n/9).
 * https://oeis.org/A392037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 45, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392037: read the infinite table T(n,k) by downward
   antidiagonals.  The table is defined by
       T(n,0) = 1,
       T(n,k) = Π_{i=0}^{k-1} (9*i + n).
   For a non‑negative integer index n, the function returns
       a(n) = T(i,j)
   where (i,j) is the pair of coordinates of the n-th entry when the
   table is read by antidiagonals (starting from (0,0)). */

seq_t a(idx_t n)
{
    /* Find the smallest s such that (s+1)(s+2)/2 > n.
       This gives the diagonal number s (i+j = s).  The loop is very
       short (s ≤ ~200 for n ≤ 10000) and uses 128‑bit arithmetic to
       avoid overflow for larger n. */
    idx_t s = 0;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Number of elements before diagonal s. */
    idx_t before = s * (s + 1) / 2;

    /* Coordinates of the n-th element.  Within a diagonal, elements
       are enumerated with increasing row index i. */
    idx_t i = n - before;       /* row index */
    idx_t j = s - i;            /* column index */

    /* Compute T(i,j) multiplicatively: product of (9*t + i) for
       t = 0 .. j-1.  All arithmetic is performed in seq_t, which
       is an unsigned integer type or an arbitrary‑precision
       integer. */
    seq_t result = 1;
    for (idx_t t = 0; t < j; ++t)
        result *= (seq_t)(9 * t + i);

    return result;
}
