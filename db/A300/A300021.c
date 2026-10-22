/*
 * A300021  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 7.
 * https://oeis.org/A300021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1230, u32 n <= 9797, u64 n <= 9797,
 *            u128 n <= 9797, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A300021 – permutation of the natural numbers.
 *  a(n) is the smallest unused positive integer such that the
 *  cumulative sum a(1)+a(2)+…+a(n) contains at least one decimal
 *  digit 7.  The sequence is generated incrementally; the function
 *  memoises previously computed terms to answer any query in
 *  O(n) time. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Test whether the decimal representation of x contains the digit 7. */
static bool has_digit_7(seq_t x)
{
    const seq_t TEN = (seq_t)10;
    if (x == 0) return false;   /* 0 has no digit 7 */
    while (x > 0) {
        if ((x % TEN) == 7) return true;
        x = x / TEN;
    }
    return false;
}

/*--------------------------------------------------------------*/

seq_t a(idx_t n)
{
    /* Static state for memoisation and bookkeeping. */
    static seq_t *seq          = NULL;   /* sequence values: seq[0] = a(1) */
    static size_t seq_len      = 0;      /* number of computed terms   */
    static size_t seq_cap      = 0;      /* allocated capacity         */
    static bool *used          = NULL;   /* used[i] == true if integer i used */
    static size_t used_cap     = 0;      /* allocated size of used[]   */
    static seq_t current_sum   = 0;      /* sum of computed terms      */

    /* Lazy initialization on first call. */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }
    if (!used) {
        used_cap = 64;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) exit(EXIT_FAILURE);
    }

    /* Generate terms up to the requested index n. */
    while (seq_len < n) {
        /* Find the smallest unused integer that keeps a 7 digit in the
           cumulative sum. Scan from 1 each time, enlarging the `used`
           array as necessary. */
        idx_t cand = 1;
        for (;;) {
            if ((size_t)cand >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 64;
                while (new_cap <= (size_t)cand) new_cap <<= 1;
                bool *p = (bool *)realloc(used, new_cap * sizeof(bool));
                if (!p) exit(EXIT_FAILURE);
                memset(p + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
                used  = p;
                used_cap = new_cap;
            }
            if (!used[(size_t)cand]) {
                seq_t new_sum = current_sum + (seq_t)cand;
                if (has_digit_7(new_sum)) break;   /* candidate accepted */
            }
            ++cand;
        }

        /* Accept the candidate. */
        used[(size_t)cand] = true;
        current_sum += (seq_t)cand;

        /* Store the value in the sequence array. */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            seq_t *p = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!p) exit(EXIT_FAILURE);
            seq = p;
            seq_cap = new_cap;
        }
        seq[seq_len++] = (seq_t)cand;
    }

    return seq[n - 1];   /* seq[0] corresponds to a(1) */
}
