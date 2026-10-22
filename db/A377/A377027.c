/*
 * A377027  a(1) = 1. For n > 1, if a(n-1) is a novel term, a(n) = a(a(n-1)),
 *          and if a(n-1) seen k (>1) times (including a(n-1)), with k*a(n-1)
 *          < n then a(n) = k*a(n-1); else a(n) = 1.
 * https://oeis.org/A377027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 327, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A377027
   a(1) = 1.
   For n > 1, let m = a(n‑1) and k be the number of times m has occurred
   up to position n‑1 (including a(n‑1) itself).
   - If k == 1 (m is novel) then a(n) = a(m).
   - Else if k * m < n then a(n) = k * m.
   - Otherwise a(n) = 1.
   The sequence is memoised so calls can be in any order.
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq = NULL;  /* dynamically grown array of terms */
static size_t len = 0;     /* number of terms currently stored */
static size_t cap = 0;     /* allocated capacity */

/* Ensure that the array can hold at least need terms. */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) { /* under‑offset; behaviour undefined */
        return 0;
    }

    /* Initialise a(1) if the table is empty. */
    if (len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)1;
        len = 1;
    }

    /* Extend the table up to n. */
    while (len < (size_t)n) {
        const seq_t prev = seq[len - 1];

        /* Count occurrences of prev up to position len‑1. */
        size_t count = 0;
        for (size_t i = 0; i < len; ++i) {
            if (seq[i] == prev) ++count;
        }

        seq_t val;  /* value to append */

        if (count == 1) {
            /* prev is novel → a(n) = a(prev).  prev <= len‑1. */
            size_t idx = (size_t)prev;   /* cast prev to an index */
            /* Safety check: idx should be >=1 and <= len-1 */
            if (idx == 0 || idx > len) {
                /* Unexpected; fall back to 1 */
                val = (seq_t)1;
            } else {
                val = seq[idx - 1];
            }
        } else {
            /* prev has appeared count times (k > 1) */
            size_t idx = (size_t)prev;
            seq_t prod = (seq_t)count * (seq_t)idx;
            if (prod < (seq_t)((size_t)len + 1)) {
                val = prod;
            } else {
                val = (seq_t)1;
            }
        }

        /* Append the computed value. */
        ensure_capacity(len + 1);
        seq[len] = val;
        ++len;
    }

    return seq[n - 1];
}
