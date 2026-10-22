/*
 * A400893  Bipartition of the positive odd numbers depending on the parity of
 *          the 2-adic valuation A007814(3*k + 2). Here the part with odd
 *          valuation based on A400890.
 * https://oeis.org/A400893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 2*A400890(n)+1
 * Range:     u8 n <= 43, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A400890(n) is the n-th non‑negative integer k with an odd 2‑adic valuation of 3·k+2.
   The function returns a(n) = 2·k + 1, i.e. the n‑th odd number whose corresponding k
   satisfies v₂(3·k+2) odd. */

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 0; count < n; ++k) {
        /* Compute x = 3*k + 2 */
        seq_t x = (seq_t)3 * (seq_t)k + 2;

        /* Find v₂(x) (trailing zeros) */
        idx_t v = 0;
        seq_t temp = x;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++v;
        }

        /* If v is odd, this k contributes to the sequence */
        if ((v & 1) != 0) {
            ++count;
            if (count == n) {
                /* a(n) = 2*k + 1 */
                return (seq_t)2 * (seq_t)k + 1;
            }
        }
    }
    return 0; /* Unreachable for n >= 1 */
}
