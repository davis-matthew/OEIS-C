/*
 * A373784  Number of n X n periodic matrices over GF(3).
 * https://oeis.org/A373784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C_3(n,k) · 3^{k(n−k)} · |GL(n−k,3)|
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C_3(n,k) is the Gaussian binomial coefficient at q = 3. */

#include <stdlib.h>

/* Local power‐by‐squaring for seq_t (base is seq_t, exponent is idx_t). */
static seq_t pow_seq_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Pre‑compute powers of 3 up to n. */
    seq_t *pow3 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Pre‑compute |GL(r,3)| for all r = 0..n. */
    seq_t *gl = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    gl[0] = (seq_t)1;                     /* |GL(0,3)| = 1 */
    for (idx_t r = 1; r <= n; ++r) {
        seq_t prod = (seq_t)1;
        seq_t pow_r = pow3[r];             /* 3^r */
        for (idx_t j = 0; j < r; ++j)
            prod *= (pow_r - pow3[j]);     /* (3^r - 3^j) */
        gl[r] = prod;
    }

    /* Accumulate the sum over k. */
    seq_t result = (seq_t)0;
    seq_t gaussian = (seq_t)1;               /* C_3(n,0) = 1 */
    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update Gaussian coefficient: multiply by (3^{n-k+1}-1)/(3^k-1). */
            seq_t num = pow3[n - (k - 1)] - (seq_t)1;   /* 3^{n-(k-1)} - 1 */
            seq_t den = pow3[k] - (seq_t)1;             /* 3^k - 1 */
            gaussian = gaussian * num / den;
        }

        idx_t r = n - k;                       /* rank of the matrix */
        idx_t exp = k * (n - k);               /* exponent for 3^{k(n-k)} */
        seq_t pow_factor = pow_seq_uint((seq_t)3, exp);   /* 3^{k(n-k)} */

        /* Term = C_3(n,k) * 3^{k(n-k)} * |GL(r,3)| */
        seq_t term = gaussian * pow_factor;
        term *= gl[r];

        result += term;
    }

    free(pow3);
    free(gl);
    return result;
}
