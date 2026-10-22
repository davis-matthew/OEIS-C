/*
 * A354267  A Fibonacci-Pascal triangle read by rows: T(n, n) = 1, T(n, n-1) =
 *          n - 1, T(n, 0) = T(n-1, 1) and T(n, k) = T(n-1, k-1) + T(n-1, k)
 *          for 0 < k < n-1.
 * https://oeis.org/A354267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with (r+1)(r+2)/2 > n
 *            (the length of the row), and c = n - r*(r+1)/2. The triangle T
 *            is defined by T(0,0)=1, T(n,0)=Fib(n-1), T(n,n)=1, T(n,n-1)=n-1,
 *            and T(n,k)=T(n-1,k-1)+T(n-1,k) for 0<k<n-1. The following
 *            routine builds rows of the triangle up to the re...
 * Range:     u8 n <= 69, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_R 200   /* sufficient for n up to about 10000 */

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th element. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;  /* first index of row r */
    idx_t c = n - start;            /* column within row r */

    /* Row 0 is a special case. */
    if (r == 0)
        return (seq_t)1;

    /* Two static arrays are enough because r never exceeds MAX_R. */
    static seq_t prev[MAX_R];
    static seq_t curr[MAX_R];

    /* Initialise row 0. */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r. */
    for (idx_t i = 1; i <= r; ++i) {
        /* T(i,0) = T(i-1,1), but for i==1 T(1,0)=Fib(0)=0. */
        curr[0] = (i == 1) ? (seq_t)0 : prev[1];

        /* T(i,i) = 1. */
        curr[i] = (seq_t)1;

        /* Middle entries. */
        for (idx_t k = 1; k < i; ++k)
            curr[k] = prev[k - 1] + prev[k];

        /* Copy current row into prev for next iteration. */
        for (idx_t k = 0; k <= i; ++k)
            prev[k] = curr[k];
    }

    return curr[c];
}
