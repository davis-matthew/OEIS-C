/*
 * A342036  Palindromes of even length only using 0 or 1.
 * https://oeis.org/A342036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007088(n) * 10^A070939(n) + A305989(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pack computed even-length binary-digit palindromes composed of 0 and 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 explicitly */
    if (n == 0)
        return (seq_t)0;

    /* n >= 1: convert to 0-based index for the list of palindromes */
    idx_t idx = n - 1;                /* index in the list excluding a(0) */
    idx_t m   = idx + 1;              /* value for log2 calculation */

    /* Determine k such that 2^k - 1 >= idx+1 ; k corresponds to half‑length */
    idx_t k = 64 - __builtin_clzll(m);        /* k = floor_log2(m) + 1   */

    /* Position within the current block of length 2*k */
    idx_t prev_cumulative = (1ULL << (k - 1)) - 1;
    idx_t pos_in_block    = idx - prev_cumulative;   /* 0‑based */

    /* First half bits: leading 1 followed by pos_in_block bits */
    idx_t first_half = (1ULL << (k - 1)) | pos_in_block;

    /* Build decimal representation of the palindrome */
    seq_t val = 0;

    /* Append first half bits (most significant to least) */
    for (idx_t i = k; i > 0; --i)
    {
        unsigned bit = (first_half >> (i - 1)) & 1U;
        val = val * 10 + bit;
    }

    /* Append reversed bits of the first half */
    for (idx_t i = 0; i < k; ++i)
    {
        unsigned bit = (first_half >> i) & 1U;
        val = val * 10 + bit;
    }

    return val;
}
