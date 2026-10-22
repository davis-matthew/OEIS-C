/*
 * A386566  a(n) = Sum_{k=0..n-1} binomial(5*k-1,k) * binomial(5*n-5*k,n-k-1).
 * https://oeis.org/A386566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{n‑1} binomial(5*k‑1,k) * binomial(5*n‑5*k,n‑k‑1)
 *            Equivalent, faster formula: a(n) = ∑_{k=0}^{n‑1} 4^{\,n‑k‑1} *
 *            binomial(5*n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    /* allocate array for binomial(5*n, k), k = 0..n‑1 */
    seq_t *binom = (seq_t*)malloc(sizeof(seq_t) * n);
    if (!binom) return 0;          /* allocation failure → return 0 */

    binom[0] = 1;                  /* C(5*n,0) = 1 */

    /* pre‑compute binomial coefficients using the multiplicative formula */
    seq_t N5 = (seq_t)n * 5;       /* 5*n, fits in seq_t for all n used */
    for (idx_t i = 1; i < n; ++i) {
        seq_t prev = binom[i - 1];
        seq_t num  = N5 - (seq_t)i + 1;   /* 5*n - i + 1 */
        prev *= num;
        prev /= (seq_t)i;
        binom[i] = prev;
    }

    /* sum of 4^{n–k–1} * binom[ k ], k = 0..n‑1 */
    seq_t sum = 0;
    seq_t pow4 = 1;                 /* 4^{0} */
    for (idx_t k = n; k > 0; --k) { /* k runs from n‑1 down to 0 */
        sum += pow4 * binom[k - 1];
        pow4 *= 4;                  /* next power of 4 */
    }

    free(binom);
    return sum;
}
