/*
 * A346175  a(0)=0. If a(n) is a novel term, a(n+1)=a(a(n)). If a(n) has
 *          appeared before, a(n+1)=number of prior terms equal to a(n).
 * https://oeis.org/A346175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 4482, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (offset 0):
   a(0) = 0
   For n > 0:
      let p = a(n-1)
      let k = number of times p appears in a(0), …, a(n-2)
      if k == 0          → a(n) = a(p)        // p is novel
      else                → a(n) = k
   This implementation memoises all required terms up to n. */

#include <stdlib.h>

/* Static storage for the sequence and counts of values. */
static seq_t *seq = NULL;          /* sequence values */
static size_t seq_len = 0;         /* number of values stored */
static size_t seq_cap = 0;         /* allocated capacity for seq */

static idx_t *cnt = NULL;          /* occurrence counts indexed by value */
static size_t cnt_cap = 0;         /* allocated capacity for cnt */

/* Ensure that cnt can index at least value v.  New cells are zeroed. */
static void _ensure_cnt(idx_t v)
{
    if (v >= cnt_cap) {
        size_t new_cap = cnt_cap ? cnt_cap * 2 : 16;
        while (new_cap <= (size_t)v)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(cnt, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Zero the newly allocated part. */
        for (size_t i = cnt_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cnt = tmp;
        cnt_cap = new_cap;
    }
}

/* Increment the count of a value.  The value is cast to idx_t. */
static void _inc_count(idx_t v)
{
    _ensure_cnt(v);
    ++cnt[v];
}

/* Return the n‑th term of A346175 (offset 0). */
seq_t a(idx_t n)
{
    /* Lazy allocation on first use. */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        cnt_cap = 16;
        cnt = (idx_t *)malloc(cnt_cap * sizeof(idx_t));
        for (size_t i = 0; i < cnt_cap; ++i)
            cnt[i] = 0;
    }

    /* Grow the sequence up to index n. */
    while (seq_len <= (size_t)n) {
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        if (seq_len == 0) {          /* a(0) = 0 */
            seq[0] = 0;
            _inc_count((idx_t)0);
            ++seq_len;
            continue;
        }

        seq_t  prev_val = seq[seq_len - 1];
        idx_t  prev_idx = (idx_t)prev_val;            /* safe cast; values ≤ index */
        idx_t  prior    = cnt[prev_idx] - 1;           /* prior occurrences */

        seq_t new_val;
        if (prior == 0) {           /* prev_val is novel */
            /* a(prev) is already computed because prev_val ≤ seq_len‑1 */
            new_val = seq[prev_idx];
        } else {                    /* prev_val has appeared before */
            new_val = (seq_t)prior;
        }

        seq[seq_len] = new_val;
        _inc_count((idx_t)new_val);
        ++seq_len;
    }

    return seq[(size_t)n];
}
