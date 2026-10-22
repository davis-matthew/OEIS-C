/*
 * A378613  a(n) = Sum_{k=0..n} binomial(4*n+k-1,k) * binomial(n-1,n-k).
 * https://oeis.org/A378613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+k-1,k) * binomial(n-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 0:  a(0)=1, a(1)=4, a(2)=44, ... */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    seq_t b1   = 1;          /* binomial(4*n + k - 1, k)   (k=0) */
    seq_t c    = 1;          /* binomial(n-1, k-1)          (k=1) */
    seq_t four_n = (seq_t)4 * (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update b1 to binomial(4*n + k - 1, k) */
        b1 = b1 * (four_n + (seq_t)k - 1) / (seq_t)k;

        /* Add term for this k */
        sum += b1 * c;

        /* Prepare c for next iteration (k+1) */
        if (k < n) {
            c = c * ((seq_t)n - (seq_t)k) / (seq_t)k;
        }
    }
    return sum;
}
