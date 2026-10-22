/*
 * A309035  If a(n) is not a term of a(0),...,a(n-1), then a(n+1) = n - m,
 *          where m is the most recent time that some new value a(m) appeared
 *          for the first time; otherwise a(n+1) is the number of terms equal
 *          to a(n) in a(0),...,a(n-1). Start with a(0)=0, a(1)=0.
 * https://oeis.org/A309035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A309035
   Recurrence: a(0)=0, a(1)=0.
   For n≥1,
       let p = a(n).
       If p is new at index n (i.e., has not appeared before index n),
           let m be the index of the most recent first appearance of a new value that occurred strictly before n;
           then a(n+1) = n – m.
       Else
           a(n+1) is the number of occurrences of p in a(0)…a(n‑1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    seq_t key;     /* sequence value */
    idx_t first;   /* first occurrence index */
    size_t count;  /* number of occurrences seen so far */
    bool used;     /* slot used */
} Entry;

static seq_t *seq_vals  = NULL;   /* array of sequence terms   */
static size_t seq_cap   = 0;       /* capacity of seq_vals      */
static idx_t  seq_len   = 0;       /* current number of items   */

static Entry *hash_tbl  = NULL;   /* hash table for value info */
static size_t hash_cap  = 0;       /* hash table size (power of 2) */

static idx_t *new_ind   = NULL;   /* stack of indices of new values */
static size_t new_cap   = 0;       /* capacity of new_ind       */
static size_t new_len   = 0;       /* number of entries in stack */

static inline size_t hash_seq_t(seq_t x)
{
    /* simple 64‑bit mix, works for 8–128 bit seq_t */
    unsigned long long lo = (unsigned long long)x;
    unsigned long long hi = (unsigned long long)(x >> 64);
    return (size_t)(lo ^ hi);
}

static Entry *find_entry(seq_t key)
{
    size_t h = hash_seq_t(key) & (hash_cap - 1);
    for (;;) {
        if (!hash_tbl[h].used) return NULL;
        if (hash_tbl[h].key == key) return &hash_tbl[h];
        h = (h + 1) & (hash_cap - 1);
    }
}

static void insert_entry(seq_t key, idx_t idx)
{
    size_t h = hash_seq_t(key) & (hash_cap - 1);
    for (;;) {
        if (!hash_tbl[h].used) {
            hash_tbl[h].used  = true;
            hash_tbl[h].key   = key;
            hash_tbl[h].first = idx;
            hash_tbl[h].count = 1;
            return;
        }
        h = (h + 1) & (hash_cap - 1);
    }
}

static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = new_ptr;
    seq_cap  = new_cap;
}

static void ensure_new_cap(size_t needed)
{
    if (new_cap >= needed) return;
    size_t new_cap2 = new_cap ? new_cap << 1 : 16;
    if (new_cap2 < needed) new_cap2 = needed;
    idx_t *new_ptr = (idx_t *)realloc(new_ind, new_cap2 * sizeof(idx_t));
    new_ind = new_ptr;
    new_cap = new_cap2;
}

static void init_once(void)
{
    if (seq_vals != NULL) return;     /* already initialised */

    /* allocate sequence array */
    seq_cap = 16;
    seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_vals[0] = 0;
    seq_vals[1] = 0;
    seq_len   = 2;

    /* hash table */
    hash_cap = 1 << 16;   /* power of two */
    hash_tbl = (Entry *)calloc(hash_cap, sizeof(Entry));
    /* store value 0 with first=0, count=2 (indices 0 and 1) */
    insert_entry((seq_t)0, 0);
    Entry *e0 = find_entry((seq_t)0);
    e0->count = 2;

    /* stack of new indices */
    new_cap = 16;
    new_ind = (idx_t *)malloc(new_cap * sizeof(idx_t));
    new_len = 1;
    new_ind[0] = 0;   /* a(0) is considered a new value */
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    init_once();

    if (n < seq_len) return seq_vals[n];

    for (idx_t cur = seq_len; cur <= n; ++cur) {
        seq_t  prev    = seq_vals[cur - 1];
        Entry *e_prev  = find_entry(prev);

        seq_t next_val;
        if (e_prev->first == (cur - 1)) {          /* prev is new at cur-1 */
            /* m is the previous new index before cur-1 */
            idx_t m_index = new_ind[new_len - 2];
            next_val = (seq_t)(cur - 1) - (seq_t)m_index;
        } else {                                  /* prev seen before */
            next_val = (seq_t)(e_prev->count - 1);
        }

        /* store next term */
        ensure_seq_cap(cur + 1);
        seq_vals[cur] = next_val;

        /* update hash table for the new term */
        Entry *e_next = find_entry(next_val);
        if (!e_next) {                            /* new value */
            insert_entry(next_val, cur);
            /* push new index onto stack */
            ensure_new_cap(new_len + 1);
            new_ind[new_len++] = cur;
        } else {                                  /* existing value */
            e_next->count += 1;
        }
    }

    seq_len = n + 1;
    return seq_vals[n];
}
