/*
 * A330091  Inverse permutation to A329303.
 * https://oeis.org/A330091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the inverse permutation to A329303 : run lengths of
 *            binary expansion of a(n) are (r1, rW, r2, rW-1, ...)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation reconstructs the binary representation of a(n)
   by reversing the run lengths of n as described. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n == 0 is a fixed point */
    if (n == 0) return 0;

    /* find most significant bit of n (0‑based index) */
    int msb = 0;
    for (int i = 63; i >= 0; --i) {
        if ((n >> i) & 1ULL) { msb = i; break; }
    }

    /* collect run lengths of n's binary expansion into run_len[0..w-1] */
    idx_t run_len[64];
    idx_t run_count = 0;
    idx_t cur_bit = (n >> msb) & 1ULL;   /* n > 0, so this is 1 */
    idx_t cur_len = 0;

    for (int i = msb; i >= 0; --i) {
        idx_t bit = (n >> i) & 1ULL;
        if (bit == cur_bit) {
            ++cur_len;
        } else {
            run_len[run_count++] = cur_len;
            cur_bit = bit;
            cur_len = 1;
        }
    }
    run_len[run_count++] = cur_len;   /* store the last run length */

    /* form the reordered run length list out_len[0..w-1] */
    idx_t out_len[64];
    idx_t out_count = 0;
    out_len[out_count++] = run_len[0];
    int l = 1, r = (int)run_count - 1;
    while (l <= r) {
        out_len[out_count++] = run_len[r--];
        if (l <= r) out_len[out_count++] = run_len[l++];
    }

    /* rebuild the number from out_len, starting with bit 1 */
    seq_t result = 0;
    idx_t cur_out_bit = 1;
    for (idx_t i = 0; i < out_count; ++i) {
        idx_t len = out_len[i];
        result <<= len;
        if (cur_out_bit) {
            seq_t mask = ((seq_t)1 << len) - 1;
            result |= mask;
        }
        cur_out_bit ^= 1;   /* flip for next run */
    }
    return result;
}
