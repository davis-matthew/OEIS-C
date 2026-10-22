/*
 * A384773  a(1) = 1, a(2) = 1. For n > 2 if a(n-1) = k is a novel term, a(n)
 *          = a(n-1-k). Otherwise if a(n-1) is a repeat term a(n) = number of
 *          m; 1 <= m <= n-2 such that a(m) = a(n-1).
 * https://oeis.org/A384773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 667, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384773
 * a(1) = 1, a(2) = 1
 * For n > 2 let k = a(n-1).
 * If k has not appeared before, a(n) = a(n-1-k).
 * Otherwise a(n) = number of indices m, 1 <= m <= n-2, with a(m)=k.
 */
#include <stdlib.h>
#include <string.h>

static seq_t *seq = NULL;          /* 1‑based array of sequence values   */
static size_t seq_cap = 0;         /* allocated capacity of seq           */
static idx_t last = 0;             /* highest index computed so far      */

static size_t *cnt = NULL;         /* occurrence counts for each value    */
static size_t cnt_cap = 0;         /* allocated capacity of cnt           */

/* Ensure cnt can be indexed by 'needed' (inclusive). */
static void ensure_cnt_cap(size_t needed)
{
    if (cnt_cap <= needed) {
        size_t newcap = cnt_cap ? cnt_cap : 16;
        while (newcap <= needed) newcap <<= 1;
        size_t *newptr = (size_t *)realloc(cnt, newcap * sizeof(size_t));
        if (newptr == NULL) return;                    /* allocation failed */
        /* Zero the newly added part of the array. */
        if (newcap > cnt_cap)
            memset(newptr + cnt_cap, 0, (newcap - cnt_cap) * sizeof(size_t));
        cnt = newptr;
        cnt_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard against underflow */

    /* Initialise on first call. */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        memset(seq, 0, seq_cap * sizeof(seq_t));
        seq[1] = 1;
        seq[2] = 1;
        last = 2;

        ensure_cnt_cap(3);
        cnt[1] = 2;                /* value 1 has appeared twice (positions 1 and 2). */
    }

    /* If the requested index is already computed, return it. */
    if ((idx_t)n <= last) return seq[n];

    /* Grow the sequence array if necessary. */
    if ((size_t)n >= seq_cap) {
        size_t newcap = seq_cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        memset(seq + seq_cap, 0, (newcap - seq_cap) * sizeof(seq_t));
        seq_cap = newcap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t k = seq[i - 1];
        size_t k_idx = (size_t)k;
        ensure_cnt_cap(k_idx);

        if (cnt[k_idx] == 1) {            /* k is novel. */
            idx_t j = (idx_t)(i - 1) - (idx_t)k;
            seq[i] = seq[j];
        } else {                          /* k repeats. */
            seq[i] = (seq_t)(cnt[k_idx] - 1);
        }

        /* Update count for the new term. */
        seq_t val = seq[i];
        size_t val_idx = (size_t)val;
        ensure_cnt_cap(val_idx);
        cnt[val_idx] += 1;
    }

    last = n;
    return seq[n];
}
