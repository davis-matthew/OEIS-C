/*
 * A297763  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 4 neighboring 1s.
 * https://oeis.org/A297763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -3*a(n-2) +a(n-3) +6*a(n-4) -18*a(n-5) with
 *            initial values a(1)=1, a(2)=2, a(3)=12, a(4)=31, a(5)=78
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)31;
    if (n == 5) return (seq_t)78;

    /* We keep only the last five terms */
    seq_t a1 = (seq_t)1;   /* a(n-5)  */
    seq_t a2 = (seq_t)2;   /* a(n-4)  */
    seq_t a3 = (seq_t)12;  /* a(n-3)  */
    seq_t a4 = (seq_t)31;  /* a(n-2)  */
    seq_t a5 = (seq_t)78;  /* a(n-1)  */
    seq_t an;              /* a(n)    */

    for (idx_t i = 6; i <= n; ++i) {
        an = 4*a5 - 3*a4 + a3 + 6*a2 - 18*a1;
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = an;
    }
    return a5;
}
