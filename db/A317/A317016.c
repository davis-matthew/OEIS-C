/*
 * A317016  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1,k) + 7 *
 *          T(n-2,k-1) for k = 0..floor(n/2). T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = 7^k * binomial(r-k, k) where r and k satisfy n =
 *            sum_{i=0}^{r-1} (floor(i/2)+1) + k, 0 ≤ k ≤ floor(r/2)
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply by 7^k */
static seq_t pow7(idx_t k)
{
    seq_t p = 1;
    for (idx_t i = 0; i < k; ++i)
        p *= (seq_t)7;
    return p;
}

/* binomial coefficient C(n, k) */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return (seq_t)1;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k corresponding to linear index n */
    idx_t r = 0;           /* current row */
    idx_t cum = 0;         /* number of entries before row r */

    while (1) {
        idx_t row_len = (r / 2) + 1;          /* floor(r/2)+1 */
        if (cum + row_len > n) break;        /* n lies in this row */
        cum += row_len;
        ++r;
    }

    idx_t k = n - cum;          /* column within row r */

    seq_t pow_val = pow7(k);
    seq_t bin_val = binom(r - k, k);

    return pow_val * bin_val;
}
