/*
 * A379778  Triangular array read by rows, T(n,k) is the number of n X n
 *          matrices over GF(2) that converge to an idempotent at rank k,
 *          n>=0, 0<=k<=n.
 * https://oeis.org/A379778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r,k) = GaussianBinomial(r,k)_2 * 2^((r-k)*(r-1))
 * where r and k are obtained from flattening the triangular array:
 *   n = r*(r+1)/2 + k , 0 <= k <= r
 * The Gaussian binomial coefficient for q = 2 is
 *   [r choose k]_2 = ∏_{i=1..k} (2^{r-k+i} - 1) / (2^i - 1)
 *
 * (see the OEIS entry for sequence A379778)
 */

#include "../utils/big_pow.h"

static seq_t gaussian_binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;

    /* use symmetry to minimize loop length */
    if (k > n - k) k = n - k;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* numerator = 2^(n-k+i) - 1 */
        seq_t num = big_pow((seq_t)2, (seq_t)(n - k + i)) - (seq_t)1;
        /* denominator = 2^i - 1 */
        seq_t den = big_pow((seq_t)2, (seq_t)i) - (seq_t)1;
        /* The product so far is integer; multiply then divide */
        result *= num;
        result /= den;    /* exact division */
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find row r and column k of the flattened index n */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (cumulative + (r + 1) <= n) {
        cumulative += r + 1;
        ++r;
    }
    idx_t k = n - cumulative;   /* 0 <= k <= r */

    /* Gaussian binomial coefficient [r choose k]_2 */
    seq_t g = gaussian_binomial(r, k);

    /* exponent = (r - k) * (r - 1) */
    seq_t exp = (seq_t)(r - k) * (seq_t)(r - 1);

    /* 2^exponent */
    seq_t pow2 = big_pow((seq_t)2, exp);

    return g * pow2;
}
