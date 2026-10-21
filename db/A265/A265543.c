/*
 * A265543  a(n) = smallest base-2 palindrome m >= n such that every base-2
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 2.
 * https://oeis.org/A265543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A265543: smallest base‑2 palindrome m ≥ n such that every base‑2 digit of n is ≤ the corresponding digit of m.
   The returned value is the decimal integer whose decimal digits form the binary representation of m. */
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* a(0) = 0 */

    /* Length of the binary representation of n (≥ 1) */
    int len = (int)floor_log2_uint64(n) + 1;   /* n > 0 */

    /* Build the palindrome in binary form (bits stored in m_bits). */
    seq_t m_bits = 0;
    for (int i = 0; i <= (len - 1) / 2; ++i) {
        int j = len - 1 - i;
        /* Extract bits of n at the mirrored positions. */
        int bi = (int)((n >> i) & 1u);
        int bj = (int)((n >> j) & 1u);
        int b  = bi > bj ? bi : bj;           /* max(bi,bj) ensures m_bit ≥ n_bit for both positions */
        m_bits |= ((seq_t)b << i);
        m_bits |= ((seq_t)b << j);
    }

    /* Convert the binary palindrome into the required decimal representation. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)((m_bits >> i) & 1);
    }
    return result;
}
