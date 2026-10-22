/*
 * A357815  Smallest maximum degree over all maximal 2-degenerate graphs with
 *          diameter 2 and n vertices.
 * https://oeis.org/A357815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2/3*(n-1)) for n>15; for n≤20 the values are *
 *            obtained by a short linear recurrence derived from the
 *            generating * function of A357815. This code evaluates the
 *            recurrence every call
 * Range:     u8 n <= 287, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the simple closed form for all sufficiently large n.  The
     * ceiling of 2/3*(n-1) equals (2*(n-1)+2)/3 for integer arithmetic.
     */
    if (n > 20) {
        idx_t t = n - 1;
        return ((seq_t)t * ((seq_t)2) + (seq_t)2) / (seq_t)3;
    }

    /* Recurrence from the generating function
     *  G(x) = N(x) / (1 - x - x^3 + x^4),
     *  where N(x) = x^2*(1 + x + x^2 - x^3 - x^5 - x^7 + x^8 + x^9 + x^13 - x^17).
     *
     *  Let a_k be the coefficient of x^k in G(x) (a(1)=a_1 etc.).
     *  With D(x) = 1 - x - x^3 + x^4 its coefficients are
     *      d[0] = 1, d[1] = -1, d[2] = 0, d[3] = -1, d[4] = 1.
     *  Then for k ≥ 0:
     *      a_k = n_k + a_{k-1} + a_{k-3} - a_{k-4},
     *  where n_k is the k‑th coefficient of N(x).  The non‑zero
     *  n_k are: n_2=1, n_3=1, n_4=1, n_5=-1, n_7=-1, n_9=-1,
     *           n_10=1, n_11=1, n_15=1, n_19=-1.
     */

    static const long long p[20] = {
        0,  0,  1,  1,  1, -1,  0, -1,  0, -1,  1,  1,  0,  0,  0,  1,
        0,  0,  0, -1
    };
    static const long long d[5] = {1, -1, 0, -1, 1};

    long long aint[21] = {0};
    aint[0] = p[0];
    for (int k = 1; k <= 20; ++k) {
        long long tmp = (k < 20 ? p[k] : 0);
        tmp -= aint[k - 1] * d[1];       /* - d1 * a_{k-1},  d1 = -1 */
        if (k >= 3) tmp -= aint[k - 3] * d[3]; /* - d3 * a_{k-3}, d3 = -1 */
        if (k >= 4) tmp -= aint[k - 4] * d[4]; /* - d4 * a_{k-4}, d4 = 1 */
        aint[k] = tmp;
    }

    return (seq_t)aint[(int)n];
}
