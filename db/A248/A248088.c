/*
 * A248088  a(n) = Sum_{k=0..floor(n/4)} binomial(n-3k, k)*(-3)^(3k)*4^(n-4k).
 * https://oeis.org/A248088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 27*a(n-4) (n >= 4) Initial values: a(0)=1,
 *            a(1)=4, a(2)=16, a(3)=64
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)64;

    seq_t buf[4];
    buf[0] = (seq_t)1;  /* a(0) */
    buf[1] = (seq_t)4;  /* a(1) */
    buf[2] = (seq_t)16; /* a(2) */
    buf[3] = (seq_t)64; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        idx_t idx     = i & 3;          /* i mod 4  */
        idx_t prev1   = (i - 1) & 3;    /* (i-1) mod 4 */
        idx_t prev4   = (i - 4) & 3;    /* (i-4) mod 4 */
        buf[idx] = buf[prev1] * (seq_t)4 - buf[prev4] * (seq_t)27;
    }
    return buf[n & 3];
}
