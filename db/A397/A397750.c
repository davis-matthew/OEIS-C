/*
 * A397750  Number of minimum independent dominating sets in the n-path graph.
 * https://oeis.org/A397750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 1; a(3*n+1) = binomial(n+2,2); a(3*n+2) = n+2
 * Range:     u8 n <= 45, u16 n <= 951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(n,k) using only seq_t arithmetic. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;
    }
    return res;
}

/* Return the a(n) value of OEIS sequence A397750. */
seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;

    if (r == 0) {
        return (seq_t)1;
    } else if (r == 1) {
        return binom_idx(q + 2, 2);
    } else { /* r == 2 */
        return (seq_t)(q + 2);
    }
}
