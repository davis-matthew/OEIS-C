/*
 * A327689  Lexicographically earliest sequence of distinct positive terms
 *          such that for any k > 0, the sum of the first k digits does not
 *          exceed k.
 * https://oeis.org/A327689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically earliest permutation of the
 *            natural numbers such that for every k>0, the sum of the first k
 *            digits of a(1),a(2),…,a(k) does not exceed k. The computation
 *            uses a greedy search: at each step the smallest unused integer
 *            that can be appended without violating the constra...
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   valid_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_candidate.h"

/* static state for memoisation */
static seq_t *seq      = NULL;   /* stored sequence     */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* capacity of seq      */

static bool *used      = NULL;   /* used[x] = true iff x already appeared  */
static size_t used_cap = 0;      /* capacity of used       */

static int64_t diff_cur = 0;     /* sum_of_digits - number_of_digits so far */

/* ------------------------------------------------------------------- */

/* ------------------------------------------------------------------- */
/* Grow internal buffers to hold at least 'target' terms of the sequence. */
static void ensure_up_to(size_t target)
{
    while (seq_len < target) {
        seq_t chosen = 0;
        int64_t newDiff = 0;

        /* find the smallest unused admissible number */
        for (idx_t cand = 1;; ++cand) {
            size_t cand_usize = (size_t)cand;

            /* enlarge used array if necessary */
            if (cand_usize + 1 > used_cap) {
                size_t old_cap = used_cap;
                size_t new_cap = used_cap ? used_cap * 2 : 128;
                if (new_cap < cand_usize + 1)
                    new_cap = cand_usize + 1;
                bool *tmp_used =
                    (bool *)realloc(used, new_cap * sizeof(bool));
                if (!tmp_used)                /* allocation failed */
                    return;
                used = tmp_used;
                used_cap = new_cap;
                for (size_t i = old_cap; i < used_cap; ++i)
                    used[i] = false;
            }

            if (used[cand_usize])
                continue;                      /* already used */

            if (valid_candidate((seq_t)cand, diff_cur, &newDiff)) {
                chosen = (seq_t)cand;
                used[cand_usize] = true;
                break;
            }
        }

        /* append chosen number to the sequence */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp_seq =
                (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp_seq)                /* allocation failed */
                return;
            seq = tmp_seq;
            seq_cap = new_cap;
        }
        seq[seq_len++] = chosen;
        diff_cur = newDiff;               /* update running difference   */
    }
}

/* ------------------------------------------------------------------- */
/* Public function: returns the nth term of the sequence.  Indexing
   starts at 1.  Calls are allowed in any order; the sequence is
   memoised internally. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n<1 */

    size_t target = (size_t)n;
    ensure_up_to(target);
    return seq[target - 1];
}
