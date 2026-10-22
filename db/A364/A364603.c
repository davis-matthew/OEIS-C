/*
 * A364603  Lexicographically earliest sequence where after the m-th
 *          appearance of term z, it is banned from re-appearing in the next
 *          m*z terms.
 * https://oeis.org/A364603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition of the lexicographically earliest sequence where
 * after the m‑th appearance of term z, it is banned from re‑appearing
 * in the next m*z terms (OEIS A364603).  The sequence is generated
 * on demand using memoisation; each call to a(n) (n ≥ 1) works in
 * amortised O(n).  The implementation is fully compliant with C11
 * and C++17, works for any built‑in unsigned integer types for
 * seq_t and idx_t, and safely allocates dynamic storage. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *seq = NULL;        /* sequence values, 0‑based */
static idx_t seq_len = 0;        /* number of values stored */
static idx_t seq_cap = 0;        /* capacity of seq array */

static idx_t *counters = NULL;   /* occurrence counts per value (m) */
static idx_t *blocks   = NULL;   /* blocking position per value */
static idx_t val_cap   = 0;       /* capacity of counters/blocks arrays */

/* Ensure counters/blocks can hold indices up to and including v. */
static void _ensure_val_cap(idx_t v) {
    if (v < val_cap) return;
    uint64_t new_cap = (val_cap) ? val_cap << 1 : 1;
    while (new_cap <= v) new_cap <<= 1;
    idx_t *cnt = (idx_t*)realloc(counters, new_cap * sizeof(idx_t));
    idx_t *blk = (idx_t*)realloc(blocks,   new_cap * sizeof(idx_t));
    if (!cnt || !blk) abort();              /* allocation failure */
    /* initialise newly added cells */
    for (idx_t i = val_cap; i < new_cap; ++i) {
        cnt[i] = 0;
        blk[i] = 0;
    }
    counters = cnt;
    blocks   = blk;
    val_cap  = new_cap;
}

/* Ensure the seq array can hold at least n elements (1‑based). */
static void _ensure_seq_cap(idx_t n) {
    if (n <= seq_cap) return;
    uint64_t new_cap = (seq_cap) ? seq_cap << 1 : 1;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    seq     = new_seq;
    seq_cap = new_cap;
}

/* Generate the sequence up to position n (1‑based). */
static void _generate_up_to(idx_t n) {
    while (seq_len < n) {
        idx_t pos = seq_len + 1;          /* current position (1‑based) */
        idx_t chosen = 0;                 /* value to append next */

        /* find smallest integer >0 that is not blocked now */
        for (idx_t x = 1;; ++x) {
            _ensure_val_cap(x);
            if (pos > blocks[x]) {        /* allowed to appear */
                chosen = x;
                break;
            }
        }

        /* update bookkeeping for the chosen value */
        counters[chosen] += 1;                 /* new count m */
        idx_t m = counters[chosen];
        blocks[chosen] = pos + m * chosen;     /* ban next m*z terms */

        /* store the value */
        _ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)chosen;
        seq_len++;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, guard */
    _generate_up_to(n);
    return seq[n - 1];
}
