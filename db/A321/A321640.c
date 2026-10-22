/*
 * A321640  Number of (7-n)-faces of the 2_41 polytope.
 * https://oeis.org/A321640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 17520 a(1) = 144960 a(2) = 544320 a(3) = 1209600 a(4) =
 *            1209600 a(5) = 483840 a(6) = 69120 a(7) = 2160 (Number of
 *            (7‑n)-faces of the 2_41 polytope) For n > 7, a(n) = 0. The
 *            values for n ≤ 7 are obtained from the Newton forward‑difference
 *            polynomial of degree 7
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The Newton forward‑difference coefficients Δ^i a(0) for i = 0..7. */
    static const long long coeff[8] = {
        17520,          /* Δ^0 a(0)  = a(0)                     */
        127440,         /* Δ^1 a(0)  = a(1)−a(0)                 */
        271920,         /* Δ^2 a(0)  = (a(2)−a(1))−(a(1)−a(0))   */
        -6000,          /* Δ^3 a(0)  = ...                         */
        -925200,        /* Δ^4 a(0)  = ...                         */
        1795920,        /* Δ^5 a(0)  = ...                         */
        -1569360,       /* Δ^6 a(0)  = ...                         */
        -754560         /* Δ^7 a(0)  = ...                         */
    };

    if (n > 7) return 0;          /* no faces of negative dimension */

    long long result = 0;          /* all intermediate results fit in int64 */
    long long comb   = 1;          /* C(n,0) */

    for (idx_t i = 0; i <= 7; ++i) {
        if (i > n) break;         /* C(n,i)=0 for i>n */
        result += coeff[i] * comb;
        if (i < 7) {
            /* next binomial coefficient: C(n,i+1) = C(n,i)*(n-i)/(i+1) */
            comb = comb * (long long)(n - i) / (i + 1);
        }
    }
    return (seq_t)result;
}
