/*
 * A296001  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2) + ... + a(n-1)*b(0), where a(0) = 1, a(1) = 2, b(0) =
 *          3, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, b(0) = 3, a(n) = ∑_{i=0}^{n-1}
 *            a(i)·b(n-1-i), the complementary sequence b is the smallest
 *            positive integer not yet in a or b
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

/* static storage for the sequences */
static seq_t *a_seq = NULL;          /* a[0] … a[a_len-1] */
static size_t a_len = 0, a_cap = 0;
static seq_t *b_seq = NULL;          /* b[0] … b[b_len-1] */
static size_t b_len = 0, b_cap = 0;
static seq_t next_candidate = 1;     /* next integer to test for b */

/* check whether a value has already appeared in a or b */
static int _used(seq_t val) {
    for (size_t i = 0; i < a_len; ++i) if (a_seq[i] == val) return 1;
    for (size_t i = 0; i < b_len; ++i) if (b_seq[i] == val) return 1;
    return 0;
}

/* compute a(n) */
seq_t a(idx_t n) {
    /* initialise a(0) and a(1) */
    if (a_len == 0) {
        grow_seq(&a_seq, &a_cap, 1);
        a_seq[0] = (seq_t)1;
        a_len = 1;
    }
    if (a_len == 1) {
        grow_seq(&a_seq, &a_cap, 2);
        a_seq[1] = (seq_t)2;
        a_len = 2;
    }

    size_t target = (size_t)n;
    for (size_t k = a_len; k <= target; ++k) {
        /* ensure b(k‑1) exists */
        while (b_len <= k-1) {
            seq_t cand = next_candidate;
            while (_used(cand)) ++cand;
            next_candidate = cand + 1;
            grow_seq(&b_seq, &b_cap, b_len + 1);
            b_seq[b_len++] = cand;
        }

        /* compute a(k) */
        seq_t sum = 0;
        for (size_t i = 0; i < k; ++i) {
            sum += a_seq[i] * b_seq[k-1-i];
        }
        grow_seq(&a_seq, &a_cap, a_len + 1);
        a_seq[a_len++] = sum;
    }

    return a_seq[n];
}
