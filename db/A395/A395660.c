/*
 * A395660  Expansion of (Sum_{k>=0} binomial(5*k,k) * x^k)^6.
 * https://oeis.org/A395660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(k+4,4) * binomial(5*n+5, n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) using a multiplicative form.
   Works for any idx_t and returns a seq_t. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t M = 5 * n + 5;          /* M = 5*n + 5 */
    seq_t C_j = binom_seq(M, n);  /* C_j = binomial(M, n) = C_{n} */
    seq_t res   = 0;
    seq_t p4    = 1;              /* 4^k */
    seq_t D_k   = 1;              /* D_k = binomial(k+4,4) */
    idx_t j = n;                  /* index for binomial(M, j), starts at j=n */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = p4 * D_k;
        term *= C_j;
        res += term;

        if (k < n) {
            /* Update D_k for next k: D_{k+1} = D_k * (k+5) / (k+1) */
            D_k = D_k * (seq_t)(k + 5) / (seq_t)(k + 1);

            /* Update C_j for next j: C_{j-1} = C_j * j / (M - j + 1) */
            C_j = (C_j * (seq_t)j) / (seq_t)(M - j + 1);
            j -= 1;

            /* Next power of 4 */
            p4 *= 4;
        }
    }
    return res;
}
