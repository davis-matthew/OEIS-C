/*
 * A250730  Number of (1+1)X(n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 52, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250730
   Recurrence:  a(1)=9,  a(2)=22,  a(3)=50,  a(4)=114,
   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-4)   for n >= 5
   (Indexing is 1‑based; a(0)=0 for out‑of‑range calls.)
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)9;
    if (n == 2)
        return (seq_t)22;
    if (n == 3)
        return (seq_t)50;
    if (n == 4)
        return (seq_t)114;

    /* keep the last four terms only */
    seq_t a1 = (seq_t)9;    /* a(n-4) */
    seq_t a2 = (seq_t)22;   /* a(n-3) */
    seq_t a3 = (seq_t)50;   /* a(n-2) */
    seq_t a4 = (seq_t)114;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)3 * a4 - a3 - (seq_t)2 * a2 + a1;
        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
