/*
 * A326638  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shares at least three digits with a(k). Lexicographically first
 *          sequence of positive integers without duplicate terms having this
 *          property.
 * https://oeis.org/A326638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1344, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A326638 – lexicographically first sequence of positive
    integers with no repeats such that for every k: the cumulative
    sum a(1)+…+a(k) shares at least three digits with a(k).
    This implementation builds the sequence on demand by
    searching for the smallest unused integer that satisfies
    the digit‑overlap condition.  It works for any seq_t /
    idx_t combination defined by the build. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_counts.h"

/* counts how many digit occurrences are shared by x and y */
static inline unsigned int shared_digit_count(seq_t x, seq_t y)
{
    unsigned char cx[10] = {0};
    unsigned char cy[10] = {0};
    digit_counts(x, cx);
    digit_counts(y, cy);
    unsigned int common = 0;
    for (int d = 0; d < 10; ++d) {
        if (cx[d] && cy[d]) {
            common += cx[d] < cy[d] ? cx[d] : cy[d];
        }
    }
    return common;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static unsigned char *used = NULL;
    static size_t used_cap = 0;
    static seq_t sum = 0;
    static bool init = false;

    /* initialise static data on first call */
    if (!init) {
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;

        used_cap = 1024;
        used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));

        sum = 0;
        init = true;
    }

    /* extend the sequence until it contains the n‑th element */
    while (seq_len < (size_t)n) {
        idx_t candidate = 1;
        /* find the next unused candidate satisfying the property */
        while (true) {
            /* ensure the used array covers the candidate */
            if ((size_t)candidate >= used_cap) {
                size_t new_cap = used_cap * 2;
                if ((size_t)candidate >= new_cap)
                    new_cap = (size_t)candidate + 1;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
                if (!tmp) abort();   /* allocation failure */
                /* initialise new entries to 0 */
                for (size_t i = used_cap; i < new_cap; ++i)
                    tmp[i] = 0;
                used = tmp;
                used_cap = new_cap;
            }

            if (used[(size_t)candidate]) {          /* already used */
                ++candidate;
                continue;
            }

            seq_t cand_val = (seq_t)candidate;
            seq_t new_sum = sum + cand_val;

            if (shared_digit_count(new_sum, cand_val) >= 3) {
                /* accept candidate */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = cand_val;
                sum = new_sum;
                used[(size_t)candidate] = 1;
                break;      /* move to next term */
            }
            ++candidate;
        }
    }
    return seq[(size_t)n - 1];
}
