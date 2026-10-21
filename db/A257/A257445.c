/*
 * A257445  Number of (n+2) X (6+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4)  for n > 5
 * Initial values: a(1)=90, a(2)=95, a(3)=104, a(4)=116, a(5)=134
 */
seq_t a(idx_t n)
{
    /* Handle the base cases directly from the look‑up table. */
    if (n == 0) return 0;      /* n==0 is outside the OEIS offset */
    if (n <= 5)
    {
        static const seq_t init[5] = {90, 95, 104, 116, 134};
        return init[n - 1];
    }

    /* Start with the last four known values:
     * a1 = a(n-1), a2 = a(n-2), a3 = a(n-3), a4 = a(n-4)
     */
    seq_t a1 = 134;   /* a(5)   */
    seq_t a2 = 116;   /* a(4)   */
    seq_t a3 = 104;   /* a(3)   */
    seq_t a4 = 95;    /* a(2)   */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = 2 * a1;
        next -= a2;
        next += a3;
        next -= a4;

        /* Shift the window for the next iteration. */
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    return a1;
}
