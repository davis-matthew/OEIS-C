/*
 * A371559  Table read by rows: row n is the unique primitive Pythagorean
 *          quadruple (a,b,c,d) such that (a + b + c - d)/2 = n and b - a = d
 *          - c = 1.
 * https://oeis.org/A371559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(4k+1) = k+1, * a(4k+2) = k+2, * a(4k+3) = (k+1)^2 + (k+1), *
 *            a(4k+4) = (k+1)^2 + (k+1) + 1 * (k = 0,1,2,...) * The sequence
 *            is the flattened list of quadruples * (n, n+1, n^2+n, n^2+n+1)
 *            read by rows
 * Range:     u8 n <= 62, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t row = (n - 1) / 4 + 1;
    idx_t pos = (n - 1) % 4;
    seq_t r   = (seq_t)row;

    switch (pos)
    {
        case 0: return r;                     /* a = n          */
        case 1: return r + (seq_t)1;          /* b = n + 1      */
        case 2: return r * r + r;             /* c = n^2 + n    */
        case 3: return r * r + r + (seq_t)1;  /* d = n^2 + n +1*/
    }
    /* unreachable */
    return 0;
}
