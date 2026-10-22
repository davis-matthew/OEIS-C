/*
 * A372219  Four-column table read by rows: row n is the unique primitive
 *          Pythagorean quadruple (a,b,c,d) such that a < (a + b + c - d)/2 =
 *          2n(n + 1) and b = c.
 * https://oeis.org/A372219
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) reads quadruples (2*r^2-1, 4*r^2+6*r+2, 4*r^2+6*r+2,
 *            6*r^2+8*r+3) for r = floor((n-2)/4)+1; a(n) = 2*r*r-1 if
 *            (n-2)%4==0; a(n) = 4*r*r+6*r+2 if (n-2)%4==1 or 2; a(n) =
 *            6*r*r+8*r+3 if (n-2)%4==3
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* below offset, undefined */
    idx_t t = n - 2;
    idx_t r = t / 4 + 1;
    seq_t R = (seq_t)r;

    seq_t a_val = 2 * R * R - 1;
    seq_t bc_val = 4 * R * R + 6 * R + 2;
    seq_t d_val = 6 * R * R + 8 * R + 3;

    idx_t col = t % 4;
    if (col == 0)
        return a_val;
    if (col < 3)
        return bc_val;
    return d_val;
}
