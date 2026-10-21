/*
 * A250848  Number of (n+1) X (3+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1225, a(2) = 8037, a(3) = 44797, a(4) = 223933 a(n) =
 *            10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) for n ≥ 5 (All
 *            arithmetic performed in the seq_t type.)
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n<1; return 0 */

    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1225;
    if (n == 2) return (seq_t)8037;
    if (n == 3) return (seq_t)44797;
    if (n == 4) return (seq_t)223933;

    /* Compute iteratively using the recurrence.  We keep the last four
       values in a circular fashion to avoid allocating large arrays. */
    seq_t a_nm4 = (seq_t)1225;   /* a(n-4)   */
    seq_t a_nm3 = (seq_t)8037;   /* a(n-3)   */
    seq_t a_nm2 = (seq_t)44797;  /* a(n-2)   */
    seq_t a_nm1 = (seq_t)223933; /* a(n-1)   */
    seq_t a_n   = 0;             /* current a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        a_n = (seq_t)10 * a_nm1
            - (seq_t)35 * a_nm2
            + (seq_t)50 * a_nm3
            - (seq_t)24 * a_nm4;

        /* Shift the window for the next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
