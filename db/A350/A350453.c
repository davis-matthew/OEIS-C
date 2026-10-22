/*
 * A350453  Number of Latin squares of order 2n with maximum inner distance
 *          with fixed entry 1 in cell (1,1).
 * https://oeis.org/A350453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + (n^2 + (3+(-1)^n)/2)^2 for n>=3; a(1)=1; a(2)=144
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)144;
    seq_t nn   = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t plus = ((n & (seq_t)1) == 0) ? (seq_t)2 : (seq_t)1;  /* 2 if even, else 1 */
    seq_t base = nn + plus;                  /* n^2 + plus */
    seq_t sq   = base * base;                /* (n^2 + plus)^2 */
    return (seq_t)4 * (seq_t)n + sq;         /* 4*n + sq */
}
