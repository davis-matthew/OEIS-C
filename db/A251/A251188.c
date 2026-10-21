/*
 * A251188  Number of (n+2)X(2+2) 0..1 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A251188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{r=0}^{2}\Big( (⌈(n+2−r)/3⌉ + 1)^3 · (⌈(n+2−r)/3⌉ + 2)
 *            / 2 \Big)
 * Range:     u8 none, u16 n <= 3, u32 n <= 16, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t r = 0; r < 3; ++r)
    {
        /* L = ceil((n+2 - r)/3) = (n + 4 - r) / 3  (integer division) */
        idx_t L = (n + 4 - r) / 3;
        seq_t lp1 = (seq_t)(L + 1);
        seq_t t = lp1 * lp1 * lp1;   /* (L+1)^3 */
        t *= (L + 2);               /* (L+1)^3 * (L+2) */
        t /= 2;                     /* divide by 2, exact */
        result *= t;
    }
    return result;
}
