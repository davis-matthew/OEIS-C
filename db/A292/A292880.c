/*
 * A292880  Number of sequences of balls colored with at most n colors such
 *          that exactly three balls are of a color seen earlier in the
 *          sequence.
 * https://oeis.org/A292880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=4..n+3} (C(k,4)+10*C(k,5)+15*C(k,6))/(n+3-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Small binomial routine using only integer arithmetic. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k == 0 || k == n)
        return (seq_t)1;
    /* Use symmetry to reduce multiplications. */
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;          /* exact division */
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* prod holds n! / (n+3-k)!; initialise for first term k = 4. */
    seq_t prod = (seq_t)n;          /* n!/(n-1)! = n */
    idx_t kmax = n + 3;

    for (idx_t k = 4; k <= kmax; ++k) {
        seq_t num = binom_small(k, 4);
        num += (seq_t)10 * binom_small(k, 5);
        num += (seq_t)15 * binom_small(k, 6);

        seq_t term = prod * num;
        sum += term;

        if (k < kmax)                      /* update prod for next k */
            prod *= (seq_t)(n + 3 - k);
    }
    return sum;
}
