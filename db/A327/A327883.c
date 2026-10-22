/*
 * A327883  a(0)=a(1)=0; thereafter a(n+1) is the index of the earliest as yet
 *          unused occurrence of a(n) if a(n) has occurred before; otherwise
 *          a(n+1) = a(a(n)-1). Once an occurrence of a(n) has been used it
 *          cannot be used again.
 * https://oeis.org/A327883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=a(1)=0; for n>1, a(n) = index of earliest unused occurrence
 *            of a(n-1) if a(n-1) appeared before, otherwise a(n) =
 *            a(a(n-1)-1)
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static tables holding the sequence and bookkeeping data.  */
static seq_t  *seq          = NULL;  /* a[0..] */
static bool    *used         = NULL;  /* used[i] = true if index i has been used */
static idx_t   *next_occ     = NULL;  /* next occurrence of the same value, index->index */
static idx_t   *head         = NULL;  /* first occurrence for each value (index) */
static idx_t   *tail         = NULL;  /* last occurrence for each value (index) */
static idx_t   *current      = NULL;  /* first unused occurrence for each value (index) */
static idx_t   *first_occ    = NULL;  /* first occurrence index of each value, -1 if none */

static idx_t seq_cap      = 0;  /* capacity for seq, used, next_occ   */
static idx_t val_cap      = 0;  /* capacity for management of values   */
static idx_t computed     = 0;  /* number of terms already computed   */

/* Ensure that the tables can store at least `need` indices and values. */
static void ensure_capacity(idx_t need)
{
    /* grow index tables */
    if (seq_cap < need + 1) {
        idx_t newcap = seq_cap ? seq_cap << 1 : 1;
        while (newcap < need + 1) newcap <<= 1;

        seq       = (seq_t *)realloc(seq,       newcap * sizeof(seq_t));
        used      = (bool  *)realloc(used,      newcap * sizeof(bool));
        next_occ  = (idx_t *)realloc(next_occ,  newcap * sizeof(idx_t));

        for (idx_t i = seq_cap; i < newcap; ++i) {
            used[i] = false;
            next_occ[i] = (idx_t)-1;
        }
        seq_cap = newcap;
    }

    /* grow value tables (values are < current index count) */
    if (val_cap < need + 1) {
        idx_t newcap = val_cap ? val_cap << 1 : 1;
        while (newcap < need + 1) newcap <<= 1;

        head      = (idx_t *)realloc(head,      newcap * sizeof(idx_t));
        tail      = (idx_t *)realloc(tail,      newcap * sizeof(idx_t));
        current   = (idx_t *)realloc(current,   newcap * sizeof(idx_t));
        first_occ = (idx_t *)realloc(first_occ, newcap * sizeof(idx_t));

        for (idx_t i = val_cap; i < newcap; ++i) {
            head[i]      = tail[i]      = current[i] = (idx_t)-1;
            first_occ[i] = (idx_t)-1;
        }
        val_cap = newcap;
    }
}

/* Append a new occurrence of value `val` at sequence position `pos`. */
static void add_occ(idx_t val, idx_t pos)
{
    /* Ensure capacity for this value index. */
    if (val >= val_cap) ensure_capacity(val);

    next_occ[pos] = (idx_t)-1;
    if (head[val] == (idx_t)-1) {
        /* first occurrence of this value */
        head[val]      = pos;
        tail[val]      = pos;
        current[val]   = pos;
        first_occ[val] = pos;
    } else {
        /* append after the current tail */
        next_occ[tail[val]] = pos;
        tail[val] = pos;
        /* no change to current[val] */
    }
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    /* Initial conditions: a[0] = a[1] = 0. */
    if (computed == 0) {
        ensure_capacity(1);
        seq[0] = 0;
        used[0] = false;

        ensure_capacity(2);
        seq[1] = 0;
        used[1] = false;

        /* record the two initial occurrences of value 0 */
        add_occ(0, 0);
        add_occ(0, 1);

        computed = 2;
    }

    /* If the requested index is already known, return it. */
    if (n < computed) return seq[n];

    /* Ensure tables can hold indices up to n and values up to n-1. */
    if (n >= seq_cap)   ensure_capacity(n);
    if (n >= val_cap)   ensure_capacity(n);

    /* Compute new terms up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t prev_val = seq[i - 1];
        idx_t val = (idx_t)prev_val;                     /* value to inspect */

        seq_t next_val;
        /* Did this value appear earlier in the sequence? */
        if (first_occ[val] < (i - 1)) {
            /* Index case: use the earliest unused occurrence. */
            idx_t occ = current[val];
            while (occ != (idx_t)-1 && used[occ]) occ = next_occ[occ];
            next_val = (seq_t)occ;                       /* occ is guaranteed valid */
            used[occ] = true;                            /* mark as used */
        } else {
            /* Repeat case: value has not appeared before i-1. */
            /* v >= 1 because the only value that can have first_occ == i-1
               and be 0 is at i-1==0, which never occurs in this loop. */
            next_val = seq[val - 1];
        }

        seq[i] = next_val;
        add_occ((idx_t)next_val, i);
    }

    computed = n + 1;
    return seq[n];
}
