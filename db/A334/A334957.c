/*
 * A334957  Triangular array read by rows. T(n,k) is the number of labeled
 *          digraphs on n nodes with exactly k self loops, n>=0, 0<=k<=n.
 * https://oeis.org/A334957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * 2^(r*(r-1)), where * r = floor((sqrt(8*n
 *            + 1) - 1) / 2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 21, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static seq_t binomial_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;

    seq_t binom = binomial_small(r, k);
    seq_t power = pow2_exp(r * (r - 1));

    return binom * power;
}
