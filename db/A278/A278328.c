/*
 * A278328  Numbers n such that abs(n - rev(n)) is a square, where rev(n) is
 *          the decimal reverse of n (A004086).
 * https://oeis.org/A278328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 9049, u64 n <= 9049,
 *            u128 n <= 9049, big any n
 * Helpers:   ensure_capacity, floor_sqrt, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A278328: numbers n such that |n - rev(n)| is a perfect square. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/rev.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *seq = NULL;          /* array of sequence terms */
    static size_t seq_len = 0;         /* number of terms stored */
    static size_t seq_cap = 0;         /* allocated capacity */
    static idx_t next_candidate = 0;   /* next integer to test */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        idx_t cand = next_candidate;
        idx_t rev_cand = rev(cand);
        idx_t diff = cand >= rev_cand ? cand - rev_cand : rev_cand - cand;
        seq_t diff_seq = (seq_t)diff;
        seq_t sq = floor_sqrt(diff_seq);

        if (sq * sq == diff_seq) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                ensure_capacity(new_cap, &seq_cap, (void **)&seq, sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cand;
        }
        next_candidate = cand + 1;
    }

    return seq[n - 1];
}
