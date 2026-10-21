/*
 * A274036  a(n) is the thickness of n (see Comments section for definition).
 * https://oeis.org/A274036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum coefficient magnitude of the square of the *
 *            binary‑polynomial B_n(x) that has a 1–coefficient exactly at the
 *            bit * positions of n
 * Range:     u8 n <= 9983, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 has no 1‑bits → thickness 0. */
    if (n == 0) {
        return 0;
    }

    /* Record the positions of the set bits of n (at most 64 positions). */
    idx_t pos[64];
    idx_t pcount = 0;
    for (idx_t i = 0; i < 64; ++i) {
        if (n & ((idx_t)1 << i)) {
            pos[pcount++] = i;
        }
    }

    /* Count how many ordered pairs of positions sum to each possible value.
     * The largest possible sum is 63 + 63 = 126, so 128 slots suffice. */
    seq_t freq[128];
    for (idx_t k = 0; k < 128; ++k) {
        freq[k] = 0;
    }

    for (idx_t i = 0; i < pcount; ++i) {
        for (idx_t j = 0; j < pcount; ++j) {
            idx_t sum = pos[i] + pos[j];
            freq[sum] += 1;
        }
    }

    /* The thickness is the maximum count among all sums. */
    seq_t result = 0;
    for (idx_t k = 0; k < 128; ++k) {
        if (freq[k] > result) {
            result = freq[k];
        }
    }
    return result;
}
