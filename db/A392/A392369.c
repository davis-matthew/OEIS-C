/*
 * A392369  Sum of odd-indexed parts of the reversed n-th composition in
 *          standard order.
 * https://oeis.org/A392369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum of parts at odd indices of the reversed n-th
 *            composition in standard order. * For n ≥ 0 set bits of n (LSB
 *            first) give 1‑based positions p₁ < p₂ < … < p_k. * Let extended
 *            = [0, p₁, p₂, …, p_k]. The differences d_j = extended[j] –
 *            extended[j‑1] form a list * of parts. The reversed compo...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t prev = 0;      /* previous position, initially 0 */
    idx_t diff_idx = 0;  /* index in the differences list */

    for (idx_t i = 0; n; ++i) {
        if (n & 1) {            /* bit at position i (0‑based) is set */
            idx_t pos = i + 1;  /* 1‑based position of this set bit */
            seq_t d = (seq_t)(pos - prev);
            if ((diff_idx & 1) == 0) {  /* even index → odd part after reversal */
                sum += d;
            }
            prev   = pos;
            ++diff_idx;
        }
        n >>= 1;                /* move to next bit */
    }
    return sum;
}
