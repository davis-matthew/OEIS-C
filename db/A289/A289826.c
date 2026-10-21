/*
 * A289826  a(n) is the smallest integer not yet in the sequence that shares
 *          exactly 2 distinct digits with the previous term a(n-1) with a(1)
 *          = 10.
 * https://oeis.org/A289826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(1) = 10;
   for n > 1: a(n) = the smallest integer not yet appearing in the sequence
   that shares exactly two distinct digits with a(n-1) and is not a repdigit
   (a number whose decimal expansion consists of only one repeated digit).
*/
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* static storage for the sequence; 20000 terms are enough for all test cases */
    static seq_t seq[20000];
    static size_t seq_len = 0;

    /* initialise the sequence on first call */
    if (seq_len == 0) {
        seq[0] = 10;          /* a(1) = 10 */
        seq_len = 1;
    }

    /* if the requested term is already computed, return it */
    if (n <= seq_len)
        return seq[n - 1];

    /* helper arrays for digit frequencies */
    unsigned int prev_freq[10];
    unsigned int cand_freq[10];

    /* build the sequence up to n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];

        /* frequency of digits in previous term */
        digit_frequencies(prev, prev_freq);

        seq_t cand = 1;        /* smallest candidate to try */
        for (;;) {
            /* 1. check if candidate has already been used */
            bool used = false;
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] == cand) {
                    used = true;
                    break;
                }
            }
            if (used) { ++cand; continue; }

            /* 2. compute candidate's digit frequencies */
            digit_frequencies(cand, cand_freq);

            /* 3. reject repdigits (only one distinct digit) */
            unsigned int distinct = 0;
            for (int d = 0; d < 10; ++d)
                if (cand_freq[d])
                    ++distinct;
            if (distinct == 1) { ++cand; continue; }

            /* 4. count how many digits are common with previous term */
            unsigned int common = 0;
            for (int d = 0; d < 10; ++d)
                if (prev_freq[d] && cand_freq[d])
                    ++common;
            if (common == 2) {
                /* good candidate found; append it */
                seq[seq_len++] = cand;
                break;
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
