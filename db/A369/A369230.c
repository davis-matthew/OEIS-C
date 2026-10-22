/*
 * A369230  Expansion of (1/x) * Series_Reversion( x * (1-x)^3 / (1-x+x^2)^3
 *          ).
 * https://oeis.org/A369230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(3*n+3, k) *
 *            binomial(n-k-1, n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return 1;

    const idx_t maxk = n / 2;      /* floor(n/2) */
    seq_t sum = 0;                  /* accumulator for the sum */

    /* binomial(3*n+3, k) – will be updated progressively */
    seq_t binom1 = 1;               /* C(3n+3, 0) = 1 */
    /* C(n-k-1, k-1) – will be updated progressivly as A_k */
    seq_t A_k = 1;                  /* A_1 = C(n-2,0) = 1 for k=1 */

    /* pre‑compute constant 3*n+3 as seq_t */
    seq_t three_n3 = (seq_t)(3 * n + 3);

    /* iterate over k = 1 .. maxk  (k = 0 contributes 0 anyway) */
    for (idx_t k = 1; k <= maxk; ++k) {
        /* Update binom1: C(3n+3, k) = C(3n+3, k-1) * (3n+3-(k-1)) / k */
        seq_t num1 = three_n3 - (seq_t)(k - 1);
        seq_t den1 = (seq_t)k;
        binom1 = binom1 * num1 / den1;

        /* Add current term to the sum */
        sum += binom1 * A_k;

        /* Prepare A_k for next k (only if we are not at the last term) */
        if (k < maxk) {
            /* A_{k+1} = A_k * ((n-2k)*(n-2k-1)) / (k*(n-k-1)) */
            seq_t num  = (seq_t)(n - 2 * k) * (seq_t)(n - 2 * k - 1);
            seq_t den  = (seq_t)k * (seq_t)(n - k - 1);
            A_k = A_k * num / den;
        }
    }

    /* Final division by (n+1) */
    return sum / ((seq_t)(n + 1));
}
