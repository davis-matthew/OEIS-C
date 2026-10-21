/*
 * A273373  Squares ending in digit 6.
 * https://oeis.org/A273373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10*n - 3*(-1)^n -5)^2/4
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* sequence is defined for n ≥ 1 */
    seq_t x = (seq_t)5 * n;  /* compute 5*n safely */
    if (n & 1)              /* n odd */
        x -= 1;
    else                    /* n even */
        x -= 4;
    return x * x;           /* a(n) = x² */
}
