/*
 * A362445  a(n) = (n+1)^4 written in base n.
 * https://oeis.org/A362445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = representation of (n+1)^4 in base n as a decimal integer
 * Range:     u8 none, u16 none, u32 none, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base‑1 (unary) special case */
    if (n == 1u) {
        seq_t d = 1;
        /* d = (n+1)^4 = 2^4 = 16 */
        for (idx_t i = 0; i < 4; ++i) d *= (seq_t)2;
        seq_t result = 0;
        for (idx_t i = 0; i < d; ++i) result = result * 10 + (seq_t)1;
        return result;
    }

    /* for n >= 2 */
    seq_t base = (seq_t)(n + 1);
    seq_t pow4 = base * base * base * base;       /* (n+1)^4 */

    /* convert pow4 to base n digits */
    seq_t digits[10];   /* sufficient: max 7 digits for n >= 2 */
    idx_t len = 0;
    seq_t temp = pow4;
    seq_t modn = (seq_t)n;

    while (temp > (seq_t)0) {
        digits[len++] = temp % modn;
        temp /= modn;
    }

    /* build decimal value from most significant digit first */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        result = result * 10 + digits[len - 1 - i];
    }
    return result;
}
