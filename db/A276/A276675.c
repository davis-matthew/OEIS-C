/*
 * A276675  A repetition-resistant sequence.
 * https://oeis.org/A276675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 766, u16 n <= 766, u32 n <= 766, u64 n <= 766,
 *            u128 n <= 766, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 0.
   For n > 1 let k be the largest value among a(1), …, a(n‑1).
   Then a(n) is the smallest non‑negative integer x such that
   the subsequence a(n‑k), a(n‑k+1), …, a(n) does not appear
   earlier in the sequence. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* sequence is defined for n ≥ 1 */

    /* Static, memoised state */
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t len = 0;        /* number of terms already stored */
    static seq_t maxprev = 0;    /* largest value among seq[0..len-1] */

    /* Extend the sequence up to position n */
    while (len < n) {
        /* We are about to compute a(len+1) */
        size_t kk = (size_t)maxprev;   /* current k */
        seq_t cand = 0;

        for (;;) {
            bool conflict = false;

            if (kk == 0) { /* mex of previous values */
                for (idx_t j = 0; j < len; ++j) {
                    if (seq[j] == cand) { conflict = true; break; }
                }
            } else if (len > kk) {
                idx_t start_last = len - kk;          /* start of last k elems */
                idx_t max_start = len - kk - 1;       /* last start index for earlier subseq */

                for (idx_t j0 = 0; j0 <= max_start; ++j0) {
                    /* compare the k elements */
                    bool equal = true;
                    for (idx_t t = 0; t < kk; ++t) {
                        if (seq[start_last + t] != seq[j0 + t]) {
                            equal = false; break;
                        }
                    }
                    if (equal && seq[j0 + kk] == cand) {
                        conflict = true; break;
                    }
                }
            }

            if (!conflict) break;
            ++cand;
        }

        /* Ensure storage */
        if ((size_t)(len + 1) > cap) {
            grow_seq(&seq, &cap, (size_t)(len + 1));
        }
        seq[len] = cand;
        ++len;
        if (cand > maxprev) maxprev = cand;
    }

    return seq[n - 1];
}
