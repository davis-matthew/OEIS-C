/*
 * A245962  Triangle read by rows: T(n,k) is the number of induced subgraphs
 *          of the Lucas cube Lambda(n) that are isomorphic to the hypercube
 *          Q(k).
 * https://oeis.org/A245962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 92, u32 n <= 327, u64 n <= 1165,
 *            u128 n <= 4440, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the (n)-th element of the triangle T(n,k) read by rows.
 * T(n,k) = Σ_{i=k..⌊n/2⌋} [ 2*binomial(n-i,i) – binomial(n-i-1,i) ] * binomial(i,k)
 * The row index r is found by cumulatively summing the row lengths 1+⌊r/2⌋ until
 * the desired n falls within that row; column k = n - Σ_{i<r}(1+⌊i/2⌋).
 * For n = 0 or 1, a(n) = 1.
 */

#include <stdint.h>

/* helper: multiplicative binomial coefficient, works for small n */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* compute T(n,k) for n >= 2 */
static seq_t triangle_T(idx_t n, idx_t k)
{
    idx_t m = n / 2;
    seq_t sum = (seq_t)0;
    for (idx_t i = k; i <= m; ++i) {
        seq_t bin_n_i = binom_idx(n - i, i);
        seq_t bin_n_i_minus1 = (n - i >= 1) ? binom_idx(n - i - 1, i) : (seq_t)0;
        seq_t A = (seq_t)2 * bin_n_i - bin_n_i_minus1;
        seq_t bin_i_k = binom_idx(i, k);
        sum += A * bin_i_k;
    }
    return sum;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find the row r and column k corresponding to index n */
    idx_t cum = 0;
    idx_t r = 0, k = 0;
    for (r = 0; ; ++r) {
        idx_t len = 1 + r / 2;
        if (n < cum + len) {
            k = n - cum;
            break;
        }
        cum += len;
    }

    if (r == 0 || r == 1) {
        return (seq_t)1;
    }
    return triangle_T(r, k);
}
