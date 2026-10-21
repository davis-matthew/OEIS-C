/*
 * A296978  List of normal sequences ordered first by length and then
 *          lexicographically, where a finite sequence is normal if it spans
 *          an initial interval of positive integers.
 * https://oeis.org/A296978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the nth decimal digit in the concatenation of all normal
 *            sequences. A normal sequence is a finite list of positive
 *            integers that contains every integer from 1 to its maximum value
 *            at least once. The sequences are listed first by length, then
 *            lexicographically. This function finds the digit...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    // Safety: n >= 1
    idx_t total_digits = 0;         // total digits produced so far
    for (idx_t len = 1; ; ++len) {  // length of sequences
        int L = (int)len;           // small enough for arrays
        // Generate all sequences of length L over alphabet 1..L
        int seq[32];
        for (int i = 0; i < L; ++i) seq[i] = 1;   // start with all ones
        bool done = false;
        while (!done) {
            /* Check normality */
            bool used[33] = {0};    // L <= 32 for our search range
            int maxUsed = 0;
            for (int i = 0; i < L; ++i) {
                int v = seq[i];
                if (v > maxUsed) maxUsed = v;
                used[v] = true;
            }
            bool normal = true;
            for (int v = 1; v <= maxUsed; ++v) {
                if (!used[v]) { normal = false; break; }
            }

            if (normal) {
                /* Compute number of decimal digits in this sequence */
                idx_t seq_digits = 0;
                for (int i = 0; i < L; ++i) {
                    seq_digits += (idx_t)digit_len((seq_t)seq[i]);
                }

                if (total_digits + seq_digits >= n) {
                    /* The desired digit lies inside this sequence */
                    idx_t offset = n - total_digits - 1;  // 0‑based within the sequence
                    for (int i = 0; i < L; ++i) {
                        seq_t val = (seq_t)seq[i];
                        int lenv = digit_len(val);
                        if (offset < (idx_t)lenv) {
                            /* Extract the offset‑th digit of val (most significant first) */
                            char buf[32];
                            int bpos = 0;
                            seq_t tmp = val;
                            while (tmp > 0) {
                                buf[bpos++] = '0' + (int)(tmp % 10);
                                tmp /= 10;
                            }
                            int digit_index = bpos - 1 - (int)offset;
                            return (seq_t)(buf[digit_index] - '0');
                        } else {
                            offset -= (idx_t)lenv;
                        }
                    }
                    /* Should never reach here */
                } else {
                    total_digits += seq_digits;
                }
            }

            /* Increment sequence in lexicographic order */
            int pos = L - 1;
            while (pos >= 0) {
                if (++seq[pos] <= L) break;
                seq[pos] = 1;
                --pos;
            }
            if (pos < 0) done = true;
        }
        /* If the string is infinite this loop will never terminate. */
    }
    /* The function should never reach this point. */
    return 0;
}
