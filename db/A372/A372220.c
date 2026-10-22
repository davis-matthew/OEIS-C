/*
 * A372220  Four-column table read by rows: row n is the unique primitive
 *          Pythagorean quadruple (a,b,c,d) such that a > (a + b + c - d)/2 =
 *          2n(n + 1) and b = c.
 * https://oeis.org/A372220
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = let m = (n-2)/4 + 2; // the row number in the table let r
 *            = (n-2) % 4; // column index 0..3 (a,b,c,d) a(n) = 2*m*m+4*m+1
 *            if r==0 4*m*m+2*m if r==1 or r==2 6*m*m+4*m+1 if r==3
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)                    // sequence is undefined for n<2
        return (seq_t)0;

    /* Compute the row number 'm' and column index 'r' */
    idx_t k = n - 2;
    idx_t m = k / 4 + 2;          /* row number, fits in idx_t */
    idx_t r = k % 4;              /* column: 0=a, 1=b, 2=c, 3=d */

    seq_t sm = (seq_t)m;          /* convert to seq_t once */
    seq_t val;

    switch (r) {
        case 0: /* a = 2m^2 + 4m + 1 */
            val = 2 * sm * sm + 4 * sm + 1;
            break;
        case 1: /* b = 4m^2 + 2m */
        case 2:
            val = 4 * sm * sm + 2 * sm;
            break;
        case 3: /* d = 6m^2 + 4m + 1 */
            val = 6 * sm * sm + 4 * sm + 1;
            break;
        default:
            val = (seq_t)0;        /* should never happen */
            break;
    }
    return val;
}
