/*
 * A350292  Triangle read by rows: the n-th row gives the saturated vertex
 *          Turán numbers for the cube graph Q_n.
 * https://oeis.org/A350292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350292: Triangle read by rows.
   a(n) returns the n-th term of the triangle where
   row m (m≥1) consists of T(m,k) for k=1..m.
   The values satisfy
      T(m,m)          = 1
      T(m,1)          = m                  if m≤2
                      = 3 * 2^(m-2)        if m≥3
      T(m,k) (2≤k≤m-1)= m * 2^(m-k-1)
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row m (1-indexed) that contains position n. */
    idx_t low = 1;
    idx_t high = n;            /* n ≤ 10 000 → m ≤ ~141, safe */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* mid*(mid+1)/2 fits in 64 bits for these constraints. */
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                         /* row index */
    idx_t t_prev = (m - 1) * m / 2;        /* T_{m-1} */
    idx_t k = n - t_prev;                  /* column position (1-based) */

    if (k == m)                           /* last element of a row */
        return (seq_t)1;

    if (k == 1) {                         /* first element */
        if (m <= 2)
            return (seq_t)m;              /* T(1,1)=1, T(2,1)=2 */
        /* m ≥ 3 : T(m,1) = 3 * 2^(m-2) */
        seq_t val = (seq_t)3;
        val <<= (m - 2);                   /* multiply by 2^(m-2) */
        return val;
    }

    /* 2 ≤ k ≤ m−1: T(m,k) = m * 2^(m-k-1) */
    seq_t val = (seq_t)m;
    /* m-k-1 ≥ 0 because k < m and k ≥ 2 */
    val <<= (m - k - 1);
    return val;
}
