/*
 * A268318  Irregular triangle read by rows: T(n,k) gives the row sums in the
 *          table Fib(n+1) X Fib(n), where k = 1..Fib(n+1), and 1's are
 *          assigned to cells on the longest diagonal path.
 * https://oeis.org/A268318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Algorithm:  a(n) is the row sum of the “Fibonacci rectangle’’ whose
   size is Fib(k+1) × Fib(k).  For a given n we locate the integer k
   such that the index lies between the cumulative sums of these
   rectangles:
          F(k+2) – 1 ≤ n ≤ F(k+3) – 2
   Let r = n – (F(k+2) – 1)  (0‑based row number within that rectangle).
   The rectangle has H = Fib(k+1) rows and W = Fib(k) columns.
   In this rectangle a monotone path from the top left to the bottom right
   uses exactly H–1 down moves and W–1 right moves.  Each row receives
   one “1” for the mandatory down move; an extra “1” is added in a row
   whenever a right move is taken while staying in that row.
   The rows that receive an extra 1 are precisely the rows
   0 ≤ r < H for which
          floor((r+1) * W / H) – floor(r * W / H) = 1  (adjusted when the
          division is exact).  Consequently the row sum is
          a(n) = 1 + (cnt(r+1) – cnt(r)),
   where cnt(x) = floor(x * W / H) − (x * W % H == 0 ? 1 : 0)
   (with cnt(0) = 0).  This computes the exact value for all n ≥ 0.
*/

#include <stddef.h>

seq_t a(idx_t n) {
    /* static Fibonacci table up to enough terms */
    static idx_t fib[70];
    static size_t fiblen = 0;
    if (fiblen == 0) {
        fib[0] = 0;
        fib[1] = 1;
        fiblen = 2;
    }

    idx_t k = n + 1;                     /* to locate the rectangle */
    /* Grow Fibonacci table until the last value exceeds k */
    while (fiblen < 70 && fib[fiblen - 1] <= k) {
        fib[fiblen] = fib[fiblen - 1] + fib[fiblen - 2];
        ++fiblen;
    }

    /* m is the largest index with fib[m] <= k */
    size_t m = fiblen - 2;          /* fib[m] <= k < fib[m+1] */
    size_t rect = m > 2 ? m - 2 : 0; /* rectangle index k-2 */

    /* Current rectangle dimensions */
    idx_t H = fib[rect + 1];        /* rows = Fib(k+1) */
    idx_t W = fib[rect];            /* cols = Fib(k)   */

    /* Offset of this rectangle in the global index */
    idx_t offset = fib[rect + 2] - 1;  /* sum of previous rectangles */
    idx_t r = n - offset;              /* row number within rectangle */

    if (W == 0) return 0;              /* n == 0 case */

    /* Compute cnt(r+1) and cnt(r) using integer arithmetic */
    idx_t rPlus = r + 1;

    idx_t prod1 = rPlus * W;
    idx_t div1  = prod1 / H;
    idx_t rem1  = prod1 % H;
    idx_t cnt1  = div1 - (rem1 == 0 ? 1 : 0);

    idx_t cnt0;
    if (r == 0) {
        cnt0 = 0;
    } else {
        idx_t prod0 = r * W;
        idx_t div0  = prod0 / H;
        idx_t rem0  = prod0 % H;
        cnt0 = div0 - (rem0 == 0 ? 1 : 0);
    }

    seq_t ans = 1 + (cnt1 - cnt0);
    return ans;
}
