/*
 * A323943  Trapezoidal matrix T(n,k) (n>=1, 1<=k<=n+2) read by rows, arising
 *          in enumeration of unbranched k-4-catafusenes.
 * https://oeis.org/A323943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A323943 – Trapezoidal matrix T(n,k) read by rows.
 *  T(1,1)=1, T(1,2)=2, T(1,3)=1
 *  Recurrence:   T(n+1,k)=3*T(n,k)+T(n,k-1)   (T(n,0)=0)
 * The function a(n) returns the n‑th term of the flattened matrix
 * (rows of increasing length r+2).  We generate rows until the
 * cumulative count reaches n, keeping only the current and previous
 * rows in static storage to avoid dynamic allocation of derived
 * integer classes.  This method works for any seq_t type supplied
 * by the build (built‑in unsigned types or an arbitrary‑precision
 * class).  Complexity is O(n) and memory usage is bounded by a
 * small, compile‑time constant.
 */
#include <stddef.h>

#define MAX_ROW 200   /* sufficient for n up to ~100 000 (rows ~ 447) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    static seq_t prev[MAX_ROW];      /* previous row, index r-1 */
    static seq_t curr[MAX_ROW];      /* current row, index r      */
    idx_t cumulative = 0;            /* number of terms processed */

    for (idx_t r = 1;; ++r) {
        idx_t row_len = r + 2;                          /* length of this row */

        if (row_len > MAX_ROW) return 0;                /* safety guard */

        if (r == 1) {                                   /* seed row 1 */
            curr[0] = (seq_t)1;
            curr[1] = (seq_t)2;
            curr[2] = (seq_t)1;
        } else {
            for (idx_t k = 0; k < row_len; ++k) {
                seq_t val = 0;
                /* 3 * prev[k]  if k exists in previous row   */
                if (k < (idx_t)(r + 1)) val += (seq_t)3 * prev[k];
                /*  prev[k‑1]   if k‑1 exists in previous row */
                if (k > 0 && (k - 1) < (idx_t)(r + 1))
                    val += prev[k - 1];
                curr[k] = val;
            }
        }

        cumulative += row_len;

        if (cumulative >= n) {                         /* target in this row */
            idx_t col = n - (cumulative - row_len);    /* 1‑based column */
            return curr[col - 1];
        }

        /* prepare for next row: copy current row into prev */
        for (idx_t k = 0; k < row_len; ++k) prev[k] = curr[k];
    }
}
