/*
 * A244503  Number of ways to place 5 points on an n X n X n triangular grid
 *          so that no pair of them has distance sqrt(3).
 * https://oeis.org/A244503
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/3840*n^10 + 1/768*n^9 - 13/384*n^8 - 7/384*n^7 +
 *            1589/768*n^6 - 24619/3840*n^5 - 1561/32*n^4 + 20965/64*n^3 -
 *            11101/240*n^2 - 85143/20*n + 9711 for n >= 7, equivalent Newton
 *            forward expression with starting point n = 7
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 21, u64 n <= 192, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for n = 4,5,6 where the polynomial is not valid */
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)339;
    if (n == 6) return (seq_t)3606;

    /* For n >= 7 use Newton forward differences with starting index 7 */
    const seq_t delta[11] = {
        (seq_t)24474,   /* Δ^0  (a(7))          */
        (seq_t)96603,   /* Δ^1  (a(8)-a(7))     */
        (seq_t)257673,  /* Δ^2  */
        (seq_t)481410,  /* Δ^3  */
        (seq_t)641976,  /* Δ^4  */
        (seq_t)613935,  /* Δ^5  */
        (seq_t)417501,  /* Δ^6  */
        (seq_t)196980,  /* Δ^7  */
        (seq_t)61320,   /* Δ^8  */
        (seq_t)11340,   /* Δ^9  */
        (seq_t)945      /* Δ^10 (constant)      */
    };

    idx_t m = n - (idx_t)7;          /* distance from starting index */
    seq_t result = 0;
    seq_t comb   = 1;               /* binomial coefficient C(m, i) */

    for (int i = 0; i <= 10; ++i) {
        result += delta[i] * comb;
        if (i == 10) break;
        /* Compute next binomial coefficient: C(m, i+1) = C(m,i) * (m-i)/(i+1) */
        comb = comb * (seq_t)(m - i) / (seq_t)(i + 1);
    }

    return result;
}
