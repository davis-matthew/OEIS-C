/*
 * A301730  Expansion of
 *          (x^8-x^7+x^6+5*x^5+4*x^4+3*x^3+5*x^2+5*x+1)/(x^6-x^5-x+1).
 * https://oeis.org/A301730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For k ≥ 1 (n = 5k): a(n) = 24k For all n = 5k + 1: a(n)
 *            = 24k + 6 For n = 5k + 2: a(2) = 11; for k > 0, a(n) = 24k + 10
 *            For n = 5k + 3: a(n) = 24k + 14 For n = 5k + 4: a(n) = 24k + 18
 *            (k = floor(n/5), 0 ≤ r < 5 where n = 5k + r)
 * Range:     u8 n <= 53, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case a(0) */
    if (n == 0) return (seq_t)1;

    /* Compute quotient and remainder upon division by 5 */
    idx_t k = n / 5;
    idx_t r = n - 5 * k;   /* r is in [0,4] */

    switch (r) {
        case 1: return (seq_t)24 * k + (seq_t)6;
        case 2:
            if (n == (idx_t)2) return (seq_t)11;
            return (seq_t)24 * k + (seq_t)10;
        case 3: return (seq_t)24 * k + (seq_t)14;
        case 4: return (seq_t)24 * k + (seq_t)18;
        default: /* r == 0 and n > 0 */
            return (seq_t)24 * k;
    }
}
