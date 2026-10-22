/*
 * A306801  An irregular fractal sequence: underline a(n) iff [a(n-1) + a(n)]
 *          is divisible by 3; all underlined terms rebuild the starting
 *          sequence.
 * https://oeis.org/A306801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 9608, u32 n <= 9608, u64 n <= 9608,
 *            u128 n <= 9608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306801 – irregular fractal sequence
   a(1) = 1, a(2) = 3.
   For n > 2, let Z = a(n‑1) and let A be the first term in the
   sequence that has not yet been duplicated.
   If (A + Z) ≡ 0 (mod 3), then a(n) = A (duplicate A).
   Otherwise a(n) is the smallest integer X not yet in the sequence
   such that (X + Z) ≠ 0 (mod 3).  The first duplicated term serviced
   is marked so it is not considered again.  The process builds the
   sequence in the order given.  The implementation remembers all
   terms, whether duplicated, and keeps track of the smallest integer
   that has not yet appeared. */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic storage for the sequence */
static seq_t  *seq      = NULL;
static size_t  seq_len   = 0;
static size_t  seq_cap   = 0;

/* flag for whether a term has already been duplicated */
static uint8_t *dup      = NULL;
static size_t  dup_cap   = 0;

/* flag for whether an integer has already appeared in the sequence */
static bool   *visited   = NULL;
static size_t  vis_cap   = 0;

/* smallest integer that has not yet appeared */
static idx_t  next_missing = 1;

/* index of the first term not yet duplicated */
static size_t first_undup = 0;

/* ensure capacity for the sequence and duplication array */
static void ensure_seq_cap(void)
{
    if (seq_len + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq,      new_cap * sizeof(seq_t));
        dup = (uint8_t *)realloc(dup,    new_cap * sizeof(uint8_t));
        seq_cap = dup_cap = new_cap;
        for (size_t i = seq_len; i < new_cap; ++i)
            dup[i] = 0;
    }
}

/* ensure capacity for the visited array up to idx (inclusive) */
static void ensure_vis_cap(idx_t idx)
{
    while (vis_cap <= (size_t)idx) {
        size_t new_cap = vis_cap ? vis_cap * 2 : 16;
        bool *new_vis = (bool *)realloc(visited, new_cap * sizeof(bool));
        visited = new_vis;
        for (size_t i = vis_cap; i < new_cap; ++i)
            visited[i] = false;
        vis_cap = new_cap;
    }
}

/* push a new term (value) onto the sequence */
static void push_term(seq_t value)
{
    ensure_seq_cap();
    seq[seq_len] = value;
    dup[seq_len] = 0;      /* new term starts as not duplicated */
    ++seq_len;

    /* mark the integer as visited */
    idx_t v = (idx_t)value;
    ensure_vis_cap(v);
    visited[v] = true;
}

/* after adding a new unique integer X, update next_missing */
static void update_next_missing(void)
{
    while (next_missing < vis_cap && visited[(idx_t)next_missing])
        ++next_missing;
}

/* initialize with first two terms */
static void initialize(void)
{
    if (seq_len > 0)
        return;
    push_term((seq_t)1);   /* a(1) */
    push_term((seq_t)3);   /* a(2) */
    first_undup = 0;
    /* initialise next_missing to the smallest non‑present integer */
    update_next_missing();
}

/* public function required by the framework */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence defined only for n ≥ 1 */

    initialize();

    while (seq_len < n) {
        seq_t last = seq[seq_len - 1];

        /* advance first_undup to the first term still not duplicated */
        while (first_undup < seq_len && dup[first_undup])
            ++first_undup;

        bool can_duplicate = (first_undup < seq_len &&
                              ((seq[first_undup] + last) % 3 == 0));

        if (can_duplicate) {
            /* duplicate the term */
            seq_t val = seq[first_undup];
            push_term(val);
            dup[first_undup] = 1;
            /* duplication does not change next_missing */
            continue;
        }

        /* find the smallest missing integer X such that (X + last) ≠ 0 (mod 3) */
        idx_t X = next_missing;
        while (true) {
            ensure_vis_cap(X);
            if (!visited[X] && ((seq_t)(X + last) % 3 != 0))
                break;
            ++X;
        }

        push_term((seq_t)X);
        /* after inserting a new integer, update next_missing */
        update_next_missing();
    }

    return seq[n - 1];
}
