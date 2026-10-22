/*
 * A383723  a(n) = A378762(A376214(n)).
 * https://oeis.org/A383723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A383723
 *  a(n) = A378762(A376214(n))
 *
 *  The sequence can be represented as a triangular array T(n,k) read
 *  by rows.  Row n has length 4n‑3 and consists of a permutation of the
 *  consecutive block [S_n , S_n + 4n‑4] where
 *      S_n = (n‑1)(2n‑3) + 1            // first value in row n
 *
 *  For 1 ≤ k ≤ 4n‑3 the entry is
 *      T(n,k) = S_n + P(n,k)
 *
 *  with m = 2n‑1 and
 *      if k < m  and k odd   :  P = m‑2 – k
 *      if k < m  and k even  :  P = k‑1
 *      if k ≥ m  and k odd   :  P = 3m‑2 – k
 *      if k ≥ m  and k even  :  P = k‑1
 *
 *  The function below maps a global index n to its row r and column
 *  position k, computes the start value S_r and P(r,k), and returns
 *  the corresponding term.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    /* find the row r such that r*(2r‑1) ≥ n */
    idx_t r = 1;
    while (r * (2 * r - 1) < n)
        ++r;

    /* previous cumulative length */
    idx_t prev = (r - 1) * (2 * (r - 1) - 1);

    /* position k within the row (1‑based) */
    idx_t k = n - prev;             /* 1 ≤ k ≤ 4r‑3 */

    /* first element of this row */
    seq_t start = (seq_t)(prev + 1);

    idx_t m = 2 * r - 1;
    idx_t offset;

    if (k < m) {
        if (k & 1)            /* odd */
            offset = m - 2 - k;
        else                  /* even */
            offset = k - 1;
    } else {                  /* k ≥ m */
        if (k & 1)            /* odd */
            offset = 3 * m - 2 - k;
        else                  /* even */
            offset = k - 1;
    }

    return start + (seq_t)offset;
}
