/*
 * A358402  a(1) = 0; for n > 1, let a(n-1) = m; if a(n-1) is the first
 *          occurrence of m then a(n) = 0, but if there is a k < n-1 with a(k)
 *          = m, a(n) is the minimum of n-1-k and j, where a(j) is the first
 *          occurrence of m in the sequence.
 * https://oeis.org/A358402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, let m = a(n-1). If the first occurrence of
 *            m is exactly at index n-1, then a(n) = 0. Otherwise, let k (<
 *            n-1) be the previous occurrence of m and j the first occurrence
 *            of m. Then a(n) = min(n-1-k, j)
 * Range:     u8 n <= 598, u16 n <= 9662, u32 n <= 9662, u64 n <= 9662,
 *            u128 n <= 9662, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Hash bucket for first occurrence lookup */
typedef struct {
    seq_t key;
    idx_t first;
    bool used;
} bucket_t;

static seq_t *seq = NULL;            /* 1‑based storage of the sequence */
static idx_t seq_len = 0;            /* largest index computed so far */
static size_t seq_cap = 0;           /* array capacity (including index 0) */

static bucket_t *first_tab = NULL;   /* hash table for first occurrence */
static size_t first_cap = 0;         /* capacity of hash table (power of 2) */
static size_t first_used = 0;        /* number of used buckets */

/* Simple hash: truncate to size_t. Works for all unsigned integral types. */
static size_t hash_seq_t(seq_t v) {
    return (size_t)(unsigned long long)v;
}

/* Ensure first_tab has space for at least 'need' entries. */
static void first_resize(size_t need) {
    size_t new_cap = first_cap ? first_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    bucket_t *new_tab = (bucket_t*)calloc(new_cap, sizeof(bucket_t));
    if (!new_tab) exit(1); /* Allocation failed */
    for (size_t i = 0; i < first_cap; ++i) {
        if (first_tab[i].used) {
            size_t h = hash_seq_t(first_tab[i].key) & (new_cap - 1);
            while (new_tab[h].used) h = (h + 1) & (new_cap - 1);
            new_tab[h] = first_tab[i];
        }
    }
    free(first_tab);
    first_tab = new_tab;
    first_cap = new_cap;
}

/* Lookup first occurrence index of 'key'; returns 0 if not found. */
static idx_t first_of(seq_t key) {
    if (!first_tab) return 0;
    size_t h = hash_seq_t(key) & (first_cap - 1);
    while (true) {
        if (!first_tab[h].used) return 0;
        if (first_tab[h].key == key) return first_tab[h].first;
        h = (h + 1) & (first_cap - 1);
    }
}

/* Insert 'key' with first occurrence index 'idx' if not already present. */
static void first_insert(seq_t key, idx_t idx) {
    if (!first_tab) { /* initialize table */
        first_resize(8);
    }
    if (first_used + 1 > first_cap / 2) {
        first_resize(first_cap * 2);
    }
    size_t h = hash_seq_t(key) & (first_cap - 1);
    while (true) {
        if (!first_tab[h].used) {
            first_tab[h].used = true;
            first_tab[h].key = key;
            first_tab[h].first = idx;
            first_used++;
            return;
        }
        if (first_tab[h].key == key) {
            /* already present – nothing to do */
            return;
        }
        h = (h + 1) & (first_cap - 1);
    }
}

/* Build the sequence up to index 'n' (inclusive).  Assumes seq_len < n. */
static void build_up_to(idx_t n) {
    if (seq == NULL) {
        /* Initial allocation */
        size_t cap = 4;
        while (cap < (size_t)n + 1) cap <<= 1;
        seq = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        if (!seq) exit(1);
        seq_cap = cap;
        seq_len = 1;
        seq[1] = 0;
        first_insert(seq[1], 1);          /* first occurrence of 0 */
    } else if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq = (seq_t*)realloc(seq, (new_cap + 1) * sizeof(seq_t));
        if (!seq) exit(1);
        seq_cap = new_cap;
    }

    for (idx_t cur = seq_len + 1; cur <= n; ++cur) {
        seq_t m = seq[cur - 1];

        /* Find previous occurrence k of m before position cur-1. */
        idx_t k = 0;
        for (long long i = (long long)cur - 2; i >= 1; --i) {
            if (seq[(idx_t)i] == m) { k = (idx_t)i; break; }
        }

        seq_t val;
        if (k == 0) {
            val = 0;          /* m is first occurrence */
        } else {
            idx_t j = first_of(m);         /* first occurrence of m */
            idx_t dist = cur - 1 - k;      /* distance to previous occurrence */
            idx_t minv = (dist < j) ? dist : j;
            val = (seq_t)minv;
        }

        seq[cur] = val;
        first_insert(val, cur);          /* record first occurrence if first time */
    }
    seq_len = n;
}

/* Public function required by the specification */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > seq_len) build_up_to(n);
    return seq[n];
}
