/*
 * A289673  Take n-th string over {1,2} in lexicographic order and apply the
 *          Post tag system described in A284116 (but adapted to the alphabet
 *          {1,2}) just once.
 * https://oeis.org/A289673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = apply the Post tag system once to the n‑th word * over
 *            {1,2} in lexicographic order. The empty word is represented * by
 *            -1
 * Range:     i8 n <= 4, i16 n <= 46, i32 n <= 814, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute 10^exp as a seq_t. */
static seq_t pow10_seq(idx_t exp)
{
    seq_t r = 1;
    while (exp--) r *= 10;
    return r;
}

#include "../utils/seq_to_str.h"
#include "../utils/seq_from_str.h"

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Step 1: find the L‑th word over {1,2} that has total rank n.
     * The words are ordered by increasing length, then lexicographically.
     * For length L there are 2^L words. */
    idx_t L = 1;
    idx_t remaining = n;
    while (remaining > ((idx_t)1 << L)) {
        remaining -= ((idx_t)1 << L);
        ++L;
    }
    idx_t word_index = remaining - 1;          /* 0‑based inside the block */

    /* Build the word as a decimal integer w_val consisting of digits 1/2. */
    seq_t w_val = 0;
    for (idx_t i = L; i > 0; --i) {
        idx_t bit = (word_index >> (i - 1)) & 1;
        seq_t digit = (seq_t)(bit + 1);         /* 0→1, 1→2 */
        w_val = w_val * 10 + digit;
    }

    /* Determine the first digit to choose the appended pattern. */
    seq_t pow10_len_minus1 = pow10_seq(L - 1);
    seq_t first_digit = (w_val / pow10_len_minus1) % 10;

    /* Set pattern and its length. */
    const char *append_str;
    idx_t append_len;
    if (first_digit == 1) {
        append_str = "11";
        append_len = 2;
    } else { /* first_digit == 2 */
        append_str = "2212";
        append_len = 4;
    }

    /* Compute combined length after appending. */
    idx_t combined_len = L + append_len;
    idx_t new_len = 0;
    if (combined_len > 3) new_len = combined_len - 3;   /* 0 means empty */

    /* If after deletion we have an empty word, return -1. */
    if (new_len == 0) {
        return -1;
    }

    /* Build the combined string w_str + append_str.   */
    /* We use seq_to_str to get the decimal representation of w_val. */
    char w_buf[256];
    size_t w_buf_len;
    seq_to_str(w_val, w_buf, &w_buf_len);

    /* Allocate a buffer for the combined string. */
    char combined[512];
    memcpy(combined, w_buf, w_buf_len);
    memcpy(combined + w_buf_len, append_str, append_len);
    combined_len = w_buf_len + append_len;

    /* Extract the suffix of length new_len (the result after deletion). */
    char suffix[512];
    memcpy(suffix, combined + (combined_len - new_len), new_len);
    suffix[new_len] = '\0';

    /* Convert the suffix back to a seq_t value. */
    seq_t result = seq_from_str(suffix);
    return result;
}
