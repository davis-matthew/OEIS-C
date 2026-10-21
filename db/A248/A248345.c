/*
 * A248345  Signed version of A094953.
 * https://oeis.org/A248345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(m,k) where m = largest integer such that m*(m+1)/2 <= n
 *            k = n - m*(m+1)/2 T(m,k) = Σ_{i=k}^{m} (i+1) * binom(i,k) *
 *            (-1)^{i-k} (binom(i,k) = i choose k) This is the coefficient of
 *            (x+1)^k in the expansion of 1 + 2x + … + (m+1)x^m
 * Range:     i8 n <= 30, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* find row index m */
    idx_t m = 0;
    /* use 128‑bit arithmetic to avoid overflow while finding m */
    while (((__int128)(m + 1) * (m + 2)) / 2 <= n)
        ++m;

    /* starting index of row m */
    idx_t row_start = (idx_t)((__int128)m * (m + 1) / 2);

    /* column index within the row */
    idx_t k = n - row_start;

    seq_t sum  = 0;
    seq_t binom = 1;          /* C(k,k) */
    seq_t sign = 1;           /* (-1)^{i-k} */

    for (idx_t i = k; i <= m; ++i) {
        if (i != k) {
            /* binom = binom * i / (i - k)   (exact division) */
            binom = binom * ((seq_t)i) / ((seq_t)(i - k));
            sign = -sign;
        }
        sum += ((seq_t)(i + 1)) * binom * sign;
    }

    return sum;
}
