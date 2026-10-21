/*
 * A244868  Number of symmetric 5 X 5 matrices of nonnegative integers with
 *          zeros on the main diagonal and every row and column adding to n.
 * https://oeis.org/A244868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24 + 94*n + 165*n^2 + 155*n^3 + 75*n^4 + 15*n^5) / 24
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative form via generating function: */
/* a(n) = Σ_{k=0}^{4} c_k * binom(n - k + 5, 5) with c = {1,16,41,16,1} */

static seq_t binom5(idx_t m)
{
    /* Return C(m,5); assumes m >= 0. */
    if (m < 5) return (seq_t)0;
    seq_t res = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        /* multiplicative binomial to avoid overflow: res = res * (m - 5 + i) / i */
        res = res * (m - 5 + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    static const seq_t coeffs[5] = {(seq_t)1, (seq_t)16, (seq_t)41, (seq_t)16, (seq_t)1};
    for (idx_t k = 0; k < 5; ++k) {
        if (n < k) continue;
        idx_t m = n - k + 5;
        sum += coeffs[k] * binom5(m);
    }
    return sum;
}
