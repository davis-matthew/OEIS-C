/*
 * A301771  Number of odd chordless cycles in the n-Andrásfai graph.
 * https://oeis.org/A301771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n+5,7) + binom(n+1,7) + 4*binom(n+2,7) -
 *            3*binom(n+3,7)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(n,k).  k is small (≤7) here.  */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (n < k) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Use the identities from the OEIS entry. */
    seq_t t1 = binom_small(n + 5, 7);   /* C(n+5,7)  */
    seq_t t2 = binom_small(n + 3, 7);   /* C(n+3,7)  */
    seq_t t3 = binom_small(n + 2, 7);   /* C(n+2,7)  */
    seq_t t4 = binom_small(n + 1, 7);   /* C(n+1,7)  */

    seq_t result = t1 + t4 + (t3 * (seq_t)4) - (t2 * (seq_t)3);
    return result;
}
