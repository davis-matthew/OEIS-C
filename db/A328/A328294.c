/*
 * A328294  Set a(1)=1 and a(2)=2. For n > 2, if a(n) had already appeared in
 *          the sequence, then a(n+1) = number of steps since its most recent
 *          appearance, as in Van Eck's sequence A181391. If a(n) had not
 *          appeared before, search instead for a(n)-1, then a(n)-2, etc.,
 *          until you find a number that has appeare...
 * https://oeis.org/A328294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2. For n>2, let v be the largest value <= a(n-1)
 *            which has already appeared. Then a(n) = (n-1) -
 *            most_recent_index_of(v) . If a(n-1) has not appeared before,
 *            search for the largest j<a(n-1) that has appeared and use its
 *            most recent index
 * Range:     u8 n <= 428, u16 n <= 8954, u32 n <= 8954, u64 n <= 8954,
 *            u128 n <= 8954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


/* Memoised, lazy computation.  Works for any seq_t and idx_t
   provided by the build.  The function is thread‑unsafe but meets
   the specification. */
static seq_t *seq = NULL;          /* a(1) stored at seq[0]          */
static size_t seq_cap = 0;         /* allocated capacity for seq      */
static size_t seq_len = 0;         /* number of terms actually stored */

static idx_t *last_idx = NULL;    /* last[i] = most recent 1‑based index of value i */
static idx_t *prev_idx = NULL;    /* prev[i] = previous 1‑based index of value i before last */
static size_t val_cap = 0;         /* capacity of the value tables (index = value) */

/* Ensure value tables can store indices up to 'need' (value). */
static void ensure_value_cap(idx_t need)
{
    size_t needed = (size_t)need + 1;   /* allocate index 0..need inclusive */
    if (needed <= val_cap) return;
    size_t new_cap = val_cap ? val_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    idx_t *tmp_last = (idx_t *)realloc(last_idx, new_cap * sizeof(idx_t));
    idx_t *tmp_prev = (idx_t *)realloc(prev_idx, new_cap * sizeof(idx_t));
    if (!tmp_last || !tmp_prev)
        abort();

    for (size_t i = val_cap; i < new_cap; ++i) {
        tmp_last[i] = 0;
        tmp_prev[i] = 0;
    }

    last_idx = tmp_last;
    prev_idx = tmp_prev;
    val_cap = new_cap;
}

/* Ensure the sequence table can hold at least 'need' elements (0‑based). */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();

    seq = tmp;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* must not happen in the sequence */

    /* Compute terms up to n on demand */
    if ((size_t)n > seq_len) {
        size_t limit = (size_t)n;
        ensure_seq_cap(limit);
        for (size_t i = seq_len + 1; i <= limit; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i == 2) {
                val = (seq_t)2;
            } else {
                /* a(i) depends on a(i-1) */
                idx_t prev_pos = (idx_t)(i - 1);
                seq_t cur_val = seq[prev_pos - 1];

                ensure_value_cap((idx_t)cur_val);

                idx_t d;
                idx_t prev_index_of_val = prev_idx[(idx_t)cur_val];
                if (prev_index_of_val != 0) {
                    /* cur_val has appeared before: use its previous occurrence */
                    d = prev_pos - prev_index_of_val;
                } else {
                    /* cur_val has not appeared: search downward for the largest
                       value <= cur_val that has appeared */
                    idx_t search = (idx_t)cur_val;
                    if (search > 0) --search;   /* start from cur_val-1 */
                    while (search > 0 && last_idx[search] == 0)
                        --search;
                    /* search must be >=1 because a(1)=1 has appeared */
                    idx_t lastpos = last_idx[search];
                    d = prev_pos - lastpos;
                }
                val = (seq_t)d;
            }

            seq[i - 1] = val;
            ensure_value_cap((idx_t)val);
            idx_t old_last = last_idx[(idx_t)val];
            prev_idx[(idx_t)val] = old_last;
            last_idx[(idx_t)val] = (idx_t)i;
        }
        seq_len = n;
    }

    return seq[n - 1];
}
