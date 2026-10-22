/*
 * A370777  Table read by rows: row n is the unique primitive Pythagorean
 *          quadruple (a,b,c,d) such that (a+b+c-d)/2 = n and a+c=d.
 * https://oeis.org/A370777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(4n-3)=1, a(4n-2)=2n, a(4n-1)=2n², a(4n)=2n²+1 (row‐wise)
 * Range:     u8 n <= 46, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the row number m (starting from 1) and the position r in the row */
    idx_t m = (n - 1) / 4 + 1;     /* row index */
    idx_t r = (n - 1) % 4;        /* position in the row */

    seq_t row = (seq_t)m;
    switch (r) {
        case 0:                 return (seq_t)1;                     /* a(4m-3) */
        case 1:                 return row * (seq_t)2;               /* a(4m-2) */
        case 2:                 return row * row * (seq_t)2;        /* a(4m-1) */
        case 3:                 return row * row * (seq_t)2 + (seq_t)1; /* a(4m) */
        default:                return (seq_t)0;                     /* unreachable */
    }
}
