/*
 * A333715  a(n) = [x^(3*n)] ( (1 + x)/(1 - x) )^n.
 * https://oeis.org/A333715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*C(3*n + k - 1, n - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 separately */
    if (n == 0) return 1;

    /* Precompute the first value of C(3*n - 1, n - 1) */
    idx_t bottom = n - 1;       /* = n-1 */
    seq_t c2 = 1;               /* current value of C(3*n + k - 1, n - 1)   */
    for (idx_t i = 1; i <= bottom; ++i) {
        /* C(3*n + k - 1, n - 1) can be built iteratively:
           C(N, r) = C(N-1, r) * (N - r) / N;  here N = 3*n + i - 1  */
        c2 = c2 * (seq_t)(2 * n + i) / (seq_t)i;
    }

    seq_t sum = 0;      /* accumulator for the binomial sum   */
    seq_t c1 = 1;       /* current value of C(n, k)            */

    for (idx_t k = 0; k <= n; ++k) {
        sum += c1 * c2;          /* add term for this k               */

        if (k == n) break;       /* finished all terms                */

        /* Update C(n, k) to C(n, k+1) */
        c1 = c1 * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update C(3*n + k - 1, n - 1) to C(3*n + (k+1) - 1, n - 1) */
        seq_t top    = (seq_t)(3 * n + k);       /* N_{k+1} */
        seq_t denom  = (seq_t)(2 * n + k + 1);   /* N_{k+1} - (n-1) */
        c2 = c2 * top / denom;
    }

    return sum;
}
