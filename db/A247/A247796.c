/*
 * A247796  From right to left in decimal representation of n: replace each
 *          maximal set of adjacent digits with their sum, if this sum is less
 *          than 10.
 * https://oeis.org/A247796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = repeatedly replace the right‑to‑left maximal block of
 *            adjacent digits whose sum < 10 (block length ≥ 2) until no such
 *            block exists
 * Range:     u8 n <= 280, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Convert n to decimal digits (least significant first). */
    unsigned char digits[25];
    int len = 0;
    seq_t t = (seq_t)n;
    while (t > 0) {
        digits[len++] = (unsigned char)(t % 10);
        t /= 10;
    }

    /* Handle the special case n == 0 after the loop. */
    if (len == 0) {
        len = 1;
        digits[0] = 0;
    }

    /* Main reduction loop. */
    while (1) {
        bool replaced = false;
        int idx = 0;

        while (idx < len) {
            int sum = 0;
            int j = idx;
            /* Build the maximal block starting at idx. */
            while (j < len && sum + digits[j] < 10) {
                sum += digits[j];
                j++;
            }

            int block_len = j - idx;
            if (block_len >= 2) {
                /* Replace block [idx .. j-1] by its sum. */
                int new_len = len - block_len + 1;
                unsigned char new_digits[25];
                int p = 0;

                for (int k = 0; k < idx; k++) new_digits[p++] = digits[k];
                new_digits[p++] = (unsigned char)sum;
                for (int k = j; k < len; k++) new_digits[p++] = digits[k];

                /* Reconstruct the new number from new_digits (LSD first). */
                seq_t new_x = 0;
                for (int k = new_len - 1; k >= 0; k--) {
                    new_x = new_x * 10 + new_digits[k];
                }

                /* Update state for the next iteration. */
                for (int k = 0; k < new_len; k++) digits[k] = new_digits[k];
                len = new_len;
                /* Continue reducing the new number. */
                replaced = true;
                break;   /* restart outer loop with updated number */
            }

            /* No replacement at this starting index; skip the whole block. */
            idx = j;
        }

        if (!replaced) break;   /* no more applicable blocks */
    }

    /* Convert digits back to seq_t. */
    seq_t result = 0;
    for (int k = len - 1; k >= 0; k--) {
        result = result * 10 + digits[k];
    }
    return result;
}
