/*
 * A259364  a(n) = 18*n^4*(2*n^3 - 23*n^2 + 38*n - 18)^2.
 * https://oeis.org/A259364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^4*(2*n^3 - 23*n^2 + 38*n - 18)^2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* Use signed 64‑bit arithmetic for the intermediate polynomial
       to avoid unsigned underflow.  All values fit in signed 64‑bit
       for the n-range given by the tests. */
    long long NL  = (long long)n;           /* n */
    long long N2  = NL * NL;                /* n^2 */
    long long N3  = N2 * NL;                /* n^3 */
    long long N4  = N2 * N2;                /* n^4 */

    /* Compute the cubic polynomial 2*n^3 - 23*n^2 + 38*n - 18. */
    long long p_signed = 2LL * N3 - 23LL * N2 + 38LL * NL - 18LL;

    /* Take absolute value before squaring; cast to seq_t only after. */
    long long p_abs_ll = p_signed < 0 ? -p_signed : p_signed;
    seq_t p_abs = (seq_t)p_abs_ll;

    /* Square of the polynomial. */
    seq_t p_sq = p_abs * p_abs;

    /* Final expression: 18*n^4*(polynomial)^2. */
    seq_t res = (seq_t)18 * (seq_t)N4 * p_sq;
    return res;
}
