/*
 * A251203  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to a nonzero multiple of 2.
 * https://oeis.org/A251203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 8*a(n-1) - 12*a(n-2) - 16*a(n-3), 
 * with a(1)=40, a(2)=180, a(3)=816. 
 * Uses 1–based indexing as in OEIS A251203. 
 */
seq_t a(idx_t n)
{
    /* Handle the offset – a(1) is the first term.  */
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)180;
    if (n == 3) return (seq_t)816;

    seq_t a0 = (seq_t)40;   /* a(1) */
    seq_t a1 = (seq_t)180;  /* a(2) */
    seq_t a2 = (seq_t)816;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = 8 * a2 - 12 * a1 - 16 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
