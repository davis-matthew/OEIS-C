/*
 * A293309  Number of magic labelings of the graph LOOP X C_10 (see comments)
 *          having magic sum n, n >= 0.
 * https://oeis.org/A293309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 112*z + 1983*z^2 + 9684*z^3 + 16120*z^4 + * 9684*z^5
 *            + 1983*z^6 + 112*z^7 + z^8) / (1 - z)^11 * expanded
 *            (coefficients of the numerator times the binomial * (1‑z)^(-11)
 *            expansion)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binom10(m) = C(m,10) for m >= 10, 0 for m < 10.
   Computed multiplicatively to keep intermediate values small. */
static seq_t binom10(idx_t m)
{
    if (m < (idx_t)10) return (seq_t)0;
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= (idx_t)10; ++i)
    {
        r = r * (seq_t)(m - (idx_t)10 + i);
        r = r / (seq_t)i;      /* exact division */
    }
    return r;
}
static const seq_t coeff[9] = {
    (seq_t)1,
    (seq_t)112,
    (seq_t)1983,
    (seq_t)9684,
    (seq_t)16120,
    (seq_t)9684,
    (seq_t)1983,
    (seq_t)112,
    (seq_t)1
};

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= (idx_t)8; ++k)
    {
        if (n < k) continue;      /* binomial argument would be < 10 */
        idx_t m = n - k + (idx_t)10;
        seq_t t = binom10(m);
        sum += coeff[k] * t;
    }
    return sum;
}
