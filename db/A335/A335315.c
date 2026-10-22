/*
 * A335315  Lexicographically earliest sequence of distinct positive integers
 *          such that the sum of digits of any consecutive pair of terms
 *          divides their consecutive concatenation.
 * https://oeis.org/A335315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   lexicographically earliest sequence of distinct positive
 *            integers such that the sum * of digits of any consecutive pair
 *            of terms divides their consecutive concatenation. * The code
 *            computes terms iteratively, caching results for efficient
 *            repeated calls
 * Range:     u8 n <= 4, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Cached sequence and used-number flags */
    static seq_t *seq   = NULL;   /* sequence values */
    static size_t seq_cap = 0;    /* capacity of seq */
    static size_t seq_len = 0;    /* number of computed terms */

    static uint8_t *used  = NULL; /* 0=unused, 1=used; index by integer value */
    static size_t  used_cap = 0;

    /* Initialization on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;
        seq_len = 1;

        used_cap = 16;
        used = (uint8_t *)calloc(used_cap, 1);
        if (!used) abort();
        used[1] = 1;
    }

    /* Return cached value if already computed */
    if (n <= seq_len)
        return seq[n-1];

    /* Extend sequence up to index n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len-1];
        seq_t digit_sum_prev = digit_sum(prev);

        /* Search for the next lexicographically smallest unused integer */
        idx_t candidate = 1;
        for (;;) {
            /* Ensure 'used' array can index 'candidate' */
            if ((size_t)candidate >= used_cap) {
                size_t new_cap = used_cap == 0 ? 16 : used_cap * 2;
                while (new_cap <= (size_t)candidate) new_cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
                if (!tmp) abort();
                memset(tmp + used_cap, 0, new_cap - used_cap);
                used = tmp;
                used_cap = new_cap;
            }

            if (!used[candidate]) {
                seq_t cand = (seq_t)candidate;
                seq_t sum_digits = digit_sum_prev + digit_sum(cand);
                if (sum_digits != 0) {
                    seq_t power10 = pow10(digit_len(cand));
                    seq_t concat = prev * power10 + cand;
                    if ((concat % sum_digits) == (seq_t)0) {
                        /* Accept the candidate */
                        if (seq_len >= seq_cap) {
                            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                            seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                            if (!new_seq) abort();
                            seq = new_seq;
                            seq_cap = new_cap;
                        }
                        seq[seq_len++] = cand;
                        used[candidate] = 1;
                        break;
                    }
                }
            }
            ++candidate;
        }
    }

    return seq[n-1];
}
