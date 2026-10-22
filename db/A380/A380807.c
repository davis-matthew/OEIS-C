/*
 * A380807  Numbers not of form a*(b+1) + b*(c+1) + c*(a+1) for 0<a<b<c.
 * https://oeis.org/A380807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 61, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers not of form a*(b+1)+b*(c+1)+c*(a+1) for 0<a<b<c */
/* Implementation uses a sieve‑like search that computes all representable
   numbers up to a limit, then builds the complement sequence. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static seq_t *seq = NULL;          /* array of non‑representable numbers */
static size_t seq_len = 0;         /* number of values stored */
static size_t seq_cap = 0;         /* capacity of seq array */

static bool *repr = NULL;          /* bitmap of representable numbers */
static idx_t repr_len = 0;         /* length of repr array (size+1) */

static idx_t current_limit = 0;    /* largest number processed so far */

/* Recompute representable numbers and rebuild seq up to new_limit */
static void recompute_for_limit(idx_t new_limit) {
    /* allocate and clear representation bitmap */
    if (repr_len < new_limit + 1) {
        repr = (bool*)realloc(repr, (new_limit + 1) * sizeof(bool));
        repr_len = new_limit + 1;
    }
    memset(repr, 0, (new_limit + 1) * sizeof(bool));

    /* Compute all representable numbers ≤ new_limit */
    for (idx_t a = 1; a < new_limit; ++a) {
        for (idx_t b = a + 1; b < new_limit; ++b) {
            idx_t c = b + 1;
            if (c >= new_limit) continue;
            seq_t S_min = (seq_t)a * (b + 1) + (seq_t)b * (c + 1) + (seq_t)c * (a + 1);
            if (S_min > new_limit) break;
            for (c = b + 1; c < new_limit; ++c) {
                seq_t S = (seq_t)a * (b + 1) + (seq_t)b * (c + 1) + (seq_t)c * (a + 1);
                if (S > new_limit) break;
                repr[(idx_t)S] = true;
            }
        }
    }

    /* Count non‑representable numbers up to new_limit */
    size_t count = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (!repr[i]) ++count;

    /* Allocate / resize seq array */
    if (seq_cap < count) {
        seq = (seq_t*)realloc(seq, count * sizeof(seq_t));
        seq_cap = count;
    }

    /* Fill seq with non‑representable numbers */
    seq_len = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (!repr[i]) seq[seq_len++] = (seq_t)i;
}

/* Return the n-th term (n≥1) of the sequence */
extern seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of bounds, return 0 */

    /* Grow the list until we have at least n entries */
    while (seq_len < n) {
        idx_t new_limit = (current_limit == 0) ? 100 : current_limit * 2;
        recompute_for_limit(new_limit);
        current_limit = new_limit;
    }

    return seq[n - 1];
}
