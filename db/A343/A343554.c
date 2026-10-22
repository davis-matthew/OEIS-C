/*
 * A343554  a(1) = 0. For n >= 1, if there exists i < j < n such that a(i) =
 *          a(j) = a(n), take the largest such i and set a(n+1) = n-i;
 *          otherwise a(n+1) = 0.
 * https://oeis.org/A343554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 349, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343554
   a(1) = 0.
   For n ≥ 1 let v = a(n).  If v has appeared at least twice before
   n, let j be the second‑to‑last index of v; then a(n+1) = n – j.
   Otherwise a(n+1) = 0. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* 1‑based array of computed terms (index 0 unused) */
static seq_t *seq      = NULL;
static size_t seq_cap  = 0;
static idx_t  seq_len  = 0;   /* number of stored terms, ≥ 1 if seq!=NULL */

/* For each value v, last1[v] = most recent index of v (≤ current i−1),
   last2[v] = second‑to‑last index of v (≤ current i−1).  Indices 0 used
   to denote “not present”. */
static idx_t *last1    = NULL;
static idx_t *last2    = NULL;
static size_t last_cap = 0;

/* Make sure seq has room for ‘needed’ 1‑based positions. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        if (newcap < needed) newcap = needed;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
}

/* Make sure the last1/last2 arrays cover indices up to ‘needed‑1’. */
static void ensure_last_cap(size_t needed)
{
    if (last_cap < needed) {
        size_t newcap = last_cap ? last_cap * 2 : 16;
        if (newcap < needed) newcap = needed;
        if (last_cap == 0) {
            last1 = (idx_t *)calloc(newcap, sizeof(idx_t));
            last2 = (idx_t *)calloc(newcap, sizeof(idx_t));
        } else {
            idx_t *tmp1 = (idx_t *)realloc(last1, newcap * sizeof(idx_t));
            idx_t *tmp2 = (idx_t *)realloc(last2, newcap * sizeof(idx_t));
            last1 = tmp1;
            last2 = tmp2;
            /* zero the newly allocated part */
            memset(last1 + last_cap, 0, (newcap - last_cap) * sizeof(idx_t));
            memset(last2 + last_cap, 0, (newcap - last_cap) * sizeof(idx_t));
        }
        last_cap = newcap;
    }
}

/* Initialise on first use. */
static void init_sequence(void)
{
    ensure_seq_cap(16);
    seq[1] = 0;          /* a(1) = 0 */
    seq_len = 1;

    ensure_last_cap(16);
    last1[0] = 0;        /* value 0 has no prior occurrence */
    last2[0] = 0;
}

seq_t a(idx_t n)
{
    if (seq == NULL)
        init_sequence();

    /* Extend the sequence up to the requested index n. */
    while (seq_len < n) {
        idx_t i = seq_len;        /* current last index (a(i)) */
        seq_t v = seq[i];

        /* Ensure the last arrays can index value v. */
        size_t vi = (size_t)v;
        if (vi >= last_cap)
            ensure_last_cap(vi + 1);

        idx_t l1 = last1[vi];
        idx_t l2 = last2[vi];

        seq_t next = (l2 != 0) ? (seq_t)(i - l2) : 0;

        /* Update occurrence information for v. */
        last2[vi] = l1;
        last1[vi] = i;

        /* Store the newly computed term. */
        ensure_seq_cap(seq_len + 2);
        seq[seq_len + 1] = next;
        seq_len++;

        /* Ensure the arrays can index the new value for future rounds. */
        size_t vn = (size_t)next;
        if (vn >= last_cap)
            ensure_last_cap(vn + 1);
    }

    return seq[n];
}
