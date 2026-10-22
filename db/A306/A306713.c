/*
 * A306713  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/(1-x^k-x^(k+1)).
 * https://oeis.org/A306713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 323, u32 n <= 1174, u64 n <= 4463,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth term of the diagonal reading of the square array
   A(n,k) = Σ_{j=0..⌊n/k⌋} binomial(j, n - k*j) ,
   where A(n,k) counts compositions of n into parts k and k+1.
   The sequence is obtained by enumerating the array by antidiagonals,
   ordering rows increasingly within each diagonal. */

static seq_t binom_uint(idx_t n, idx_t k)
{
    /* Return binomial coefficient C(n,k); if k>n return 0. */
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);  /* multiply */
        res = res / i;            /* divide – exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t prefix = d * (d + 1) / 2;
    idx_t r = n - prefix;           /* row index (0‑based) */
    idx_t k = d - r + 1;            /* column index (>=1) */

    seq_t sum = 0;
    idx_t limit = r / k;            /* floor(r/k) */
    for (idx_t j = 0; j <= limit; ++j) {
        sum += binom_uint(j, r - k * j);
    }
    return sum;
}
