/*
 * A306635  a(n) = Product_{k=1..n} BarnesG(2*k).
 * https://oeis.org/A306635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} BarnesG(2*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; behaviour below offset is irrelevant */

    seq_t a_val  = (seq_t)1;       /* a(1) = 1 */
    seq_t G_even = (seq_t)1;       /* G(2) = 1   */
    seq_t fact   = (seq_t)1;       /* holds factorial(2*k-3); starts with factorial(1) for k=2 */

    for (idx_t k = 2; k <= n; ++k) {
        /* factorial(2*k-3) is current fact */
        seq_t f3 = fact;                            /* f3 = (2*k-3)! */

        /* factorial(2*k-2) = f3 * (2*k-2) */
        seq_t f2 = fact * (seq_t)(2 * k - 2);

        /* G(2*k) = G(2(k-1)) * f2 * f3 */
        G_even = G_even * f2 * f3;

        /* accumulate product for a(n) */
        a_val = a_val * G_even;

        /* advance factorial to (2*k-1) for the next iteration:
           factorial(2*k-1) = f2 * (2*k-1) */
        fact = f2 * (seq_t)(2 * k - 1);
    }

    return a_val;
}
