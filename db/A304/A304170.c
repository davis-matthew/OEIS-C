/*
 * A304170  a(n) = 32*3^n + 18*2^n - 116 (n>=1).
 * https://oeis.org/A304170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*3^(n+1) + 18*2^(n+1) - 116 (for n >= 0)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute 3^(n+1) and 2^(n+1) by repeated multiplication by 3 and 2.  */
    seq_t pow3 = 3;          /* 3^1 */
    seq_t pow2 = 2;          /* 2^1 */
    for (idx_t i = 1; i <= n; ++i) {
        pow3 *= 3;
        pow2 *= 2;
    }
    /* Apply the closed‑form expression.  */
    return (seq_t)32 * pow3 + (seq_t)18 * pow2 - (seq_t)116;
}
