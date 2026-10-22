/*
 * A378117  Lexicographically earliest sequence of nonnegative integers a(0),
 *          a(1), ..., such that a(n) is the number of pairs of adjacent terms
 *          whose sum is n.
 * https://oeis.org/A378117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3761, u16 n <= 9154, u32 n <= 9154, u64 n <= 9154,
 *            u128 n <= 9154, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence a(n): for each n, a(n) is the number
 * of pairs of consecutive terms whose sum is n.  The values are computed
 * iteratively, always choosing the smallest admissible a(i) that keeps the
 * counts of sums consistent with the definition. */
#include <stdlib.h>
#include <stdbool.h>

/* Static tables for the sequence and its pair‑count array. */
static seq_t *seq      = NULL;           /* a(n) values */
static idx_t seq_cap  = 0;                /* capacity of seq   */
static idx_t seq_last = (idx_t)-1;        /* last computed index */

static seq_t *cnt      = NULL;           /* counts of observed pairs for each sum */
static idx_t cnt_cap  = 0;                /* capacity of cnt    */

/* Ensure the data arrays have at least new_cap elements. */
static void ensure_seq_cap(idx_t new_cap)
{
    if (new_cap <= seq_cap) return;
    idx_t old_cap = seq_cap;
    seq_cap = (new_cap > old_cap * 2) ? new_cap : old_cap * 2;
    seq_t *new_ptr = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    for (idx_t i = old_cap; i < seq_cap; ++i) new_ptr[i] = 0;
    seq = new_ptr;
}

static void ensure_cnt_cap(idx_t new_cap)
{
    if (new_cap <= cnt_cap) return;
    idx_t old_cap = cnt_cap;
    cnt_cap = (new_cap > old_cap * 2) ? new_cap : old_cap * 2;
    seq_t *new_ptr = (seq_t*)realloc(cnt, cnt_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    for (idx_t i = old_cap; i < cnt_cap; ++i) new_ptr[i] = 0;
    cnt = new_ptr;
}

/* First-time initialization of the tables. */
static void init_tables(void)
{
    if (seq != NULL) return;
    seq_cap = 2;
    seq = (seq_t*)calloc(seq_cap, sizeof(seq_t));
    if (!seq) abort();
    seq[0] = 0;
    seq_last = 0;

    cnt_cap = 2;
    cnt = (seq_t*)calloc(cnt_cap, sizeof(seq_t));
    if (!cnt) abort();
}

/* Return a(n).  The function is thread‑safe only relative to the
 * algorithm; it uses static storage and therefore is not re‑entrant. */
seq_t a(idx_t n)
{
    init_tables();

    if (n > seq_last) {
        /* Ensure the seq array has room for all indices up to n. */
        ensure_seq_cap(n + 1);

        for (idx_t i = seq_last + 1; i <= n; ++i) {
            seq_t prev = seq[i - 1];

            /* Find the smallest candidate a(i) that keeps the counts
             * of sums consistent. */
            idx_t cand_idx = 0;
            for (;;) {
                seq_t cand = (seq_t)cand_idx;
                seq_t sum = prev + cand;

                /* In practice the sums stay small enough to fit in idx_t.
                 * We cast for indexing; if the sum were larger than
                 * idx_t, the behaviour is not defined by the problem
                 * statement but this cast keeps the compiler happy. */
                idx_t sum_idx = (idx_t)sum;
                ensure_cnt_cap(sum_idx + 1);
                seq_t cnt_before = cnt[sum_idx];

                if (sum_idx <= i - 1) {
                    /* a[sum] already fixed; must respect it. */
                    seq_t a_sum = seq[sum_idx];
                    if (cnt_before + 1 > a_sum) {
                        ++cand_idx;
                        continue;
                    }
                } else if (sum_idx == i) {
                    /* sum equals current index; a[sum] will be set later,
                     * but it must be at least cnt_before + 1. */
                    seq_t required = cnt_before + 1;
                    if (cand < required) {
                        cand_idx = (idx_t)required;
                        continue;
                    }
                }

                /* candidate is acceptable */
                seq[i] = cand;
                cnt[sum_idx] = cnt_before + 1;
                break;
            }
        }
        seq_last = n;
    }
    return seq[n];
}
