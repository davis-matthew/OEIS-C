/*
 * A329388  Solution sequence (a(n)) of the complementary equation a(n) = 5
 *          b(n+1) - b(n), with b(0) = 1.
 * https://oeis.org/A329388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*b(n+1) - b(n), with complementary sequences, b(0)=1
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   ensure_used_size, mark_used, used_is_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ensure_used_size.h"
#include "../utils/mark_used.h"
#include "../utils/used_is_set.h"

/* Compute a(n) for the complementary sequence A329388. */
seq_t a(idx_t n)
{
    /* Static state across calls. */
    static seq_t *a_seq = NULL;       static size_t a_cap = 0;     static idx_t a_len = 0;
    static seq_t *b_seq = NULL;       static size_t b_cap = 0;     static idx_t b_len = 0;
    static uint8_t *used = NULL;      static idx_t used_size = 0;

    /* One‑time initialization. */
    if (a_len == 0) {
        a_cap = 16;
        a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));

        b_cap = 4;
        b_seq = (seq_t*)malloc(b_cap * sizeof(seq_t));

        /* b(0) = 1 */
        b_seq[0] = (seq_t)1;
        b_len = 1;
        mark_used(&used, &used_size, (idx_t)b_seq[0]);

        /* b(1) = 2 (smallest unused > 1) */
        seq_t cand = (seq_t)2;
        while (used_is_set(&used, &used_size, (idx_t)cand)) cand++;
        if (b_len >= b_cap) {
            b_cap *= 2;
            b_seq = (seq_t*)realloc(b_seq, b_cap * sizeof(seq_t));
        }
        b_seq[1] = cand;
        b_len = 2;
        mark_used(&used, &used_size, (idx_t)cand);

        /* a(0) = 5*b(1) - b(0) */
        seq_t a0 = 5 * b_seq[1] - b_seq[0];
        a_seq[0] = a0;
        a_len = 1;
        mark_used(&used, &used_size, (idx_t)a0);
    }

    /* Return already computed value if possible. */
    if (n < a_len) return a_seq[n];

    /* Compute terms up to n. */
    for (idx_t i = a_len; i <= n; ++i) {
        /* Ensure b(i+1) exists. */
        while (b_len <= i + 1) {
            seq_t cand = b_seq[b_len - 1] + 1;
            while (used_is_set(&used, &used_size, (idx_t)cand)) cand++;

            if (b_len >= b_cap) {
                b_cap *= 2;
                b_seq = (seq_t*)realloc(b_seq, b_cap * sizeof(seq_t));
            }
            b_seq[b_len] = cand;
            ++b_len;
            mark_used(&used, &used_size, (idx_t)cand);
        }

        seq_t val = 5 * b_seq[i + 1] - b_seq[i];
        if (a_len >= a_cap) {
            a_cap *= 2;
            a_seq = (seq_t*)realloc(a_seq, a_cap * sizeof(seq_t));
        }
        a_seq[a_len] = val;
        mark_used(&used, &used_size, (idx_t)val);
        ++a_len;
    }
    return a_seq[n];
}
