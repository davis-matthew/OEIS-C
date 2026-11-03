/*
 * A280173  a(0) = 1, a(n+1) = 2*a(n) + periodic sequence of length 2: repeat
 *          [5, -4].
 * https://oeis.org/A280173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(2n) = 3 * 4^n - 2 * a(2n+1) = 6 * 4^n + 1 * (derived from
 *            a(0)=1, a(n+1)=2*a(n)+[5,-4] recurring)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 943 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Use the even/odd formula for direct computation. */
    if ((n & 1U) == 0) {                 /* n is even: a(2n)  */
        seq_t p = pow4(n >> 1);          /* p = 4^(n/2)        */
        return 3 * p - 2;
    } else {                              /* n is odd: a(2n+1) */
        seq_t p = pow4(n >> 1);          /* p = 4^((n-1)/2)   */
        return 6 * p + 1;
    }
}
