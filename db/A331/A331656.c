/*
 * A331656  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) * n^k.
 * https://oeis.org/A331656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) * n^k
 *            Implementation uses the recurrence term_{k+1} = term_k * (n-k) /
 *            (k+1) * (n+k+1) / (k+1) * n where term_0 = 1. Each division is
 *            exact, so no loss in a fixed‑size integer type, and the same
 *            code works for the arbitrary‑precision seq_t type....
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t term = 1;
    seq_t sum  = term;            /* sum starts with term_0 = 1           */

    for (idx_t k = 0; k < n; ++k) {
        /* (k+1) will be used several times; cast once per loop */
        const seq_t denom = (seq_t)(k + 1);

        /* multiply by (n-k) and divide by (k+1) */
        term = term * (seq_t)(n - k);
        term = term / denom;

        /* multiply by (n+k+1) and divide by (k+1) again */
        term = term * (seq_t)(n + k + 1);
        term = term / denom;

        /* finally multiply by n */
        term = term * (seq_t)n;

        /* accumulate */
        sum += term;
    }

    return sum;
}
