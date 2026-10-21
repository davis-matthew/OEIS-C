/*
 * A241578  Square array read by antidiagonals upwards: T(n,k) = Sum_{j=1..k}
 *          n^(k-j)*Stirling_2(k,j) (n >= 0, k >= 1).
 * https://oeis.org/A241578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) = Σ_{j=1..k}
 *            n^(k-j) * Stirling2(k,j) by antidiagonals upwards. For n ≥ 0,
 *            let s be the smallest integer with s(s+1)/2 > n. Then n and k
 *            are given by offset = n - (s-1)*s/2 n_val = (s-1) - offset k_val
 *            = offset + 1 and a(n) = T(n_val , k_val)
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* compute T(n,k) = Σ_{j=1..k} n^(k-j) * Stirling2(k,j) */
static seq_t compute_T(idx_t n_val, idx_t k_val)
{
    /* allocate array for Stirling numbers of the second kind */
    size_t K = (size_t)k_val;
    seq_t *S = (seq_t *)malloc((K + 1) * sizeof(seq_t));
    if (!S) return 0;                 /* out of memory, return 0 */

    memset(S, 0, (K + 1) * sizeof(seq_t));
    S[1] = 1;                          /* S(1,1) = 1 */

    for (size_t m = 2; m <= K; ++m) {
        for (size_t j = m; j >= 1; --j) {
            if (j == m)
                S[j] = 1;             /* S(m,m) = 1 */
            else
                S[j] = j * S[j] + S[j-1];
        }
    }

    /* compute the weighted sum Σ S(k,j) * n^(k-j) */
    seq_t sum = 0;
    seq_t pow = 1;                     /* n^0 */
    for (idx_t j = k_val; j >= 1; --j) {
        seq_t contrib = S[j] * pow;
        sum += contrib;
        if (j > 1)
            pow *= (seq_t)n_val;      /* multiply by n for next lower j */
    }

    free(S);
    return sum;
}

/* compute a(n) for the OEIS sequence A241578 */
seq_t a(idx_t n)
{
    /* find s such that s(s+1)/2 > n */
    idx_t s = 1;
    while (((__uint128_t)s * (s + 1)) / 2 <= n)
        ++s;

    /* number of elements before diagonal s */
    idx_t before = (idx_t)((( __uint128_t)(s - 1) * s) / 2);

    idx_t offset  = n - before;
    idx_t n_val   = (s - 1) - offset;
    idx_t k_val   = offset + 1;

    return compute_T(n_val, k_val);
}
