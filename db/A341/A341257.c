/*
 * A341257  Positions of palindromes in the ordering of all 01-words defined
 *          at A341256.
 * https://oeis.org/A341257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the position of the nth binary palindrome in the
   colexicographic ordering of all 0-1 words.
   The position of a word of length L with integer value k
   (bits interpreted as a palindrome) is
       pos = (2^L - 1) + k
   where k is the integer value of the palindrome (since palindromes
   are fixed under bit-reversal, their colex rank equals k). */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but guard */

    /* Find the length L of the palindrome containing the nth one */
    seq_t cumulative = 0;
    idx_t len;
    for (len = 1; ; ++len) {
        idx_t half = (len + 1) / 2;
        seq_t count = (seq_t)1 << half;          /* 2^half */
        if (cumulative + count >= (seq_t)n) {
            /* n lies in this length */
            seq_t prev_cumulative = cumulative;
            seq_t index_in_len = (seq_t)n - prev_cumulative - 1; /* 0‑based */
            /* Build the palindrome corresponding to this index */
            /* index_in_len will fit into the prefix size – it is < 2^half */
            uint64_t p = (uint64_t)index_in_len;    /* prefix bits */
            int bits[128] = {0};
            /* first half (including middle if odd) from prefix p */
            for (int i = 0; i < (int)half; ++i) {
                bits[i] = (p >> (half - 1 - i)) & 1U;
            }
            /* mirror to the second half */
            for (int i = 0; i < (int)half; ++i) {
                bits[len - 1 - i] = bits[i];
            }
            /* combine into integer k */
            seq_t k = 0;
            for (int i = 0; i < (int)len; ++i) {
                k = (k << 1) | (seq_t)bits[i];
            }
            /* compute final position: 2^len - 1 + k */
            seq_t pos = ((seq_t)1 << len) - (seq_t)1 + k;
            return pos;
        }
        cumulative += count;
    }
}
