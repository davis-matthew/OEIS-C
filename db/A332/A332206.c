/*
 * A332206  Numbers k such that A332205(k) = 0.
 * https://oeis.org/A332206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = e + 3*A000695(k) where e = n % 2, k = n / 2, and *
 *            A000695(k) is obtained by interpreting the binary representation
 *            of k * as a number in base 4 (i.e., sum of (bit_i of k) * 4^i)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n >> 1;          /* k = n / 2 */
    seq_t e = (seq_t)(n & 1);  /* e = n % 2 */

    /* Compute A000695(k): interpret bits of k as base‑4 digits. */
    seq_t val = 0;
    seq_t base = 1;
    while (k > 0) {
        if (k & 1) {
            val += base;
        }
        base *= 4;
        k >>= 1;
    }

    return e + (seq_t)3 * val;
}
