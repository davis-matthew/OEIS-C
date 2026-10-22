/*
 * A333709  a(n) is the number of iterations for x = x + 1/x needed until x +
 *          1/x > n, starting with x = 1.
 * https://oeis.org/A333709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333709:
 * a(n) = number of iterations of x_{k+1} = x_k + 1/x_k,
 * starting with x_0 = 1, until the inequality x_k + 1/x_k > n holds.
 * (This directly follows the definition given in the OEIS entry.)
 */
seq_t a(idx_t n)
{
    /* Start at x = 1.  Use double arithmetic for speed and
     * correctness (the value of n is an integer ≤ 10 000, so its
     * double representation is exact).  The number of iterations
     * never exceeds n²/2, which for n = 10 000 is 50 000 000,
     * well within the limits of idx_t. */
    double x = 1.0;
    double limit = (double)n;
    idx_t iter = 0;
    while (x + 1.0/x <= limit) {
        x += 1.0/x;
        iter++;
    }
    return (seq_t)iter;
}
