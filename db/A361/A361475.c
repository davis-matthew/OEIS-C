/*
 * A361475  Array read by ascending antidiagonals: A(n, k) = (k^n - 1)/(k -
 *          1), with k >= 2.
 * https://oeis.org/A361475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the triangular array A(i,k) with A(i,k) =
 *            sum_{t=0}^{i-1} k^t, where k = j+2 (k ≥ 2), over all (i,j) in
 *            ascending antidiagonals (i decreasing, j increasing)
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal s such that T(s-1) ≤ n < T(s),
       where T(s) = (s+1)(s+2)/2 is the total number of
       terms up to and including diagonal s. */
    idx_t s = 0;
    while (1) {
        /* Use 128‑bit arithmetic to avoid overflow */
        unsigned __int128 t = (__int128)(s + 1) * (s + 2) / 2;
        if ((idx_t)t > n) break;
        ++s;
    }

    idx_t prev = s * (s + 1) / 2;      /* T(s‑1) */
    idx_t p    = n - prev;             /* offset inside diagonal */
    idx_t i    = s - p;                /* row index */
    idx_t j    = p;                    /* column offset (k‑2) */

    seq_t k = (seq_t)(j + 2);           /* actual k value (≥ 2) */

    if (i == 0) return 0;              /* first row yields 0 */

    seq_t term = 1;                    /* k⁰ */
    seq_t sum  = 0;

    /* Compute the geometric series 1 + k + … + k⁽ⁱ⁻¹⁾ */
    for (idx_t t = 0; t < i; ++t) {
        sum  += term;
        term *= k;
    }
    return sum;
}
