/*
 * A256266  Total number of ON states after n generations of cellular
 *          automaton based on triangles (see Comments lines for definition).
 * https://oeis.org/A256266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 6 * A255748(n) A255748(0) = 0; for n ≥ 1, A255748(n) =
 *            Σ_{k=1..n} (2^{⌊log₂ k⌋+1} – k) The difference term 2^{⌊log₂
 *            k⌋+1} – k is equal to the next power of two greater than k minus
 *            k. This is computed iteratively here
 * Range:     u8 n <= 11, u16 n <= 239, u32 n <= 9757, u64 n <= 9757,
 *            u128 n <= 9757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;          /* running total of A255748 */
    idx_t next = 2;         /* next power of two (2^{⌊log₂ k⌋+1}) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t diff = (seq_t)(next - k);  /* 2^{⌊log₂ k⌋+1} – k */
        sum += diff;

        /* Move to the next power-of-two block when k reaches next‑1 */
        if (k == next - 1)
            next <<= 1;
    }

    return (seq_t)6 * sum;   /* a(n) = 6 * A255748(n) */
}
