/*
 * A250859  Number of (7+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (31774*n^6 + 473568*n^5 + 2674189*n^4 + 8320086*n^3 * +
 *            12645859*n^2 + 8917428*n + 2359296)/36 * (This polynomial is
 *            equivalent to the recurrence *
 *            a(n)=7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7)
 *            * for n>7, with a(1)=983950, a(2)=5480917, a(3)=20067117, *
 *            a(4)=5757001...
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=0 or n<1 no term is defined; returning 0 is safe. */
    if (n < 1) return 0;

    /* Use 128‑bit arithmetic to avoid overflow on intermediate
       values.  The final result fits in the type seq_t for all
       n that the test harness will use. */
    unsigned __int128 x = n;
    unsigned __int128 val = 31774;
    val = val * x + 473568;    /* 31774*n + 473568 */
    val = val * x + 2674189;   /* ...*n + 2674189 */
    val = val * x + 8320086;   /* ...*n + 8320086 */
    val = val * x + 12645859;  /* ...*n + 12645859 */
    val = val * x + 8917428;   /* ...*n + 8917428 */
    val = val * x + 2359296;   /* ...*n + 2359296 */
    val = val / 36;            /* Divide by 36 to get the integer value */

    return (seq_t)val;
}
