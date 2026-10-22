/*
 * A390508  a(n) = index of the NA-run sequence of the n-th 01-word, where all
 *          01-words are lexicographically ordered (i.e., enumerated) as in
 *          A076478; see Comments.
 * https://oeis.org/A390508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = index of the NA‑run sequence of the n‑th 01‑word
 * in the lexicographic ordering of all binary words (A076478). */
/* The algorithm:
 *   1) Determine the length L of the n‑th 01‑word by accumulating counts
 *      of words of each length until the total exceeds n.
 *   2) Extract that word as an L‑bit value (offset).
 *   3) Scan the bits to build the NA‑run word: d(i)[c(i)] for each run.
 *   4) Compute the lexicographic index of the resulting NA‑word:
 *         index = (2^lenNA − 1) + value_of_NA_word
 */

#include <stdint.h>

/* Return the index (0‑based) of the most significant set bit of x
 * (caller guarantees x > 0). */
static inline idx_t msb_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Find the length L of the n‑th 01‑word   */
    idx_t cumulative = 0;   /* words counted so far */
    idx_t L = 0;            /* current length under consideration */

    while (n > cumulative) {
        ++L;
        cumulative += (idx_t)1 << L;          /* words of length L */
    }

    /* Total words of lengths < L */
    idx_t prev_cumulative = cumulative - ((idx_t)1 << L);

    /* 2. Offset of the word within length L group (0‑based) */
    idx_t offset = n - prev_cumulative - 1;  /* 0 <= offset < 2^L */

    /* 3. Build the NA‑run word bit by bit */
    seq_t  r_val = 0;   /* decimal value of the NA word */
    idx_t  lenNA = 0;   /* number of bits in NA word         */

    /* Process first bit (most significant) */
    int cur_bit = (int)((offset >> (L - 1)) & 1ULL);
    idx_t run_len = 1;

    for (int pos = (int)(L - 2); pos >= 0; --pos) {
        int bit = (int)((offset >> pos) & 1ULL);
        if (bit == cur_bit) {
            ++run_len;
        } else {
            /* Flush current run: output bit then binary run_len */
            r_val = (r_val << 1) | (seq_t)cur_bit;
            ++lenNA;

            /* Binary representation of run_len (no leading zeros) */
            idx_t bits_run = msb_idx(run_len) + 1;
            for (int b = (int)(bits_run - 1); b >= 0; --b) {
                int rbit = (int)((run_len >> b) & 1ULL);
                r_val = (r_val << 1) | (seq_t)rbit;
                ++lenNA;
            }

            /* Start next run */
            cur_bit = bit;
            run_len = 1;
        }
    }

    /* Flush the final run */
    r_val = (r_val << 1) | (seq_t)cur_bit;
    ++lenNA;
    idx_t bits_run = msb_idx(run_len) + 1;
    for (int b = (int)(bits_run - 1); b >= 0; --b) {
        int rbit = (int)((run_len >> b) & 1ULL);
        r_val = (r_val << 1) | (seq_t)rbit;
        ++lenNA;
    }

    /* 4. Compute the lexicographic index of the NA word
       (first count all shorter words, then add the value). */
    seq_t idx_shorter = ((seq_t)1 << lenNA) - (seq_t)1;
    return idx_shorter + r_val;
}
