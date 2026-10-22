/*
 * A373286  a(n) = Product_{k=1..n} (k^2 mod n if k^2 mod n > 0).
 * https://oeis.org/A373286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (k^2 mod n if k^2 mod n > 0)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 16, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 1;
    if (n == 0) return ans;        /* safety, though n>=1 by definition */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;          /* k^2 */
        seq_t r  = kk % (seq_t)n;                /* k^2 mod n */
        if (r > 0) {
            ans *= r;
        }
    }
    return ans;
}
