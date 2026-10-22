/*
 * A342040  Binary palindromes of odd length.
 * https://oeis.org/A342040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal integer whose decimal digits are the binary
 *            palindrome of odd length that is the nth such palindrome (n >=
 *            1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    /* Determine k: number of bits in the left half of the palindrome.
       For length L = 2k-1, there are 2^(k-1) palindromes.  Cumulative
       count up to k-1 is 2^(k-1)-1, so the first index of length k is
       2^(k-1).  We need k such that n >= 2^(k-1).  Thus k = floor_log2(n)+1. */
    idx_t k = floor_log2_uint64(n) + 1U;

    /* First index of length k */
    idx_t start = ((idx_t)1U << (k - 1U));
    /* Offset within this length: 0‑based */
    idx_t offset = n - start;

    /* Left half (k bits) with leading 1.  The lower (k-1) bits are offset. */
    seq_t left_half = ((seq_t)1U << (k - 1U)) | (seq_t)offset;

    seq_t result = 0;

    /* Append bits of left_half from most significant to least significant. */
    for (idx_t i = k; i-- > 0; ) {
        seq_t bit = (left_half >> i) & (seq_t)1U;
        result = result * (seq_t)10 + bit;
    }

    /* Append the mirror of the bits above the middle (exclude the middle bit). */
    for (idx_t i = 1; i < k; ++i) {
        seq_t bit = (left_half >> i) & (seq_t)1U;
        result = result * (seq_t)10 + bit;
    }

    return result;
}
