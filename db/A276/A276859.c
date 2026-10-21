/*
 * A276859  First differences of the Beatty sequence A022843 for e.
 * https://oeis.org/A276859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*e) - floor((n-1)*e) e ≈ 2.718281828… Approximated
 *            by the rational 271828/100000, which has error < 5·10⁻⁶. For n ≤
 *            10⁴ the product error n·|e‑P/Q| is < 0.5, so floor(n*e) computed
 *            with this approximation is exact
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset handling – not used */

    /* Use a 64‑bit intermediate to avoid overflow on small seq_t. */
    idx_t num      = n;
    idx_t num_prev = n - 1;

    /* floor(n*e) ≈ floor((n*271828)/100000) */
    idx_t floor_n      = (num * 271828ULL) / 100000ULL;
    idx_t floor_n_minus1 = (num_prev * 271828ULL) / 100000ULL;

    return (seq_t)(floor_n - floor_n_minus1);
}
