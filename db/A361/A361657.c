/*
 * A361657  Constant term in the expansion of (1 + x^2 + y^2 + 1/(x*y))^n.
 * https://oeis.org/A361657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} 1/(k!^2 * (2*k)! * (n-4*k)!)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* kmax is the largest k such that 4*k <= n */
    idx_t kmax = n / 4;
    seq_t sum = (seq_t)1;      /* term for k = 0 equals 1 */
    seq_t term = (seq_t)1;     /* term for current k, initialized to k = 0 */

    seq_t base_n = (seq_t)n;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* Compute the product (n - 4(k-1)) * (n - 4(k-1) - 1) *
           (n - 4(k-1) - 2) * (n - 4(k-1) - 3) */
        seq_t shift = (seq_t)4 * (seq_t)(k - 1);
        seq_t a1 = base_n - shift;
        seq_t a2 = a1 - 1;
        seq_t a3 = a2 - 1;
        seq_t a4 = a3 - 1;

        seq_t num = a1 * a2;
        num = num * a3;
        num = num * a4;

        /* Denominator: k^2 * (2*k-1) * (2*k) */
        seq_t denom = (seq_t)k * (seq_t)k;
        seq_t b1 = (seq_t)((idx_t)2 * k - 1);
        seq_t b2 = (seq_t)((idx_t)2 * k);
        denom = denom * b1;
        denom = denom * b2;

        /* Update term for this k and add to the sum */
        term = term * num / denom;
        sum += term;
    }

    return sum;
}
