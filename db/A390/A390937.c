/*
 * A390937  Number of entries in the map T after the n-th iteration, when
 *          initially T[1] = 1 is the only entry, and in subsequent
 *          iterations, T[v] := k + (T[v] if defined else 0) for each
 *          key-value pair (k, v) in the map.
 * https://oeis.org/A390937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 45, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A390937
 *
 *  Sequence definition:
 *      T[1] = 1 initially.
 *      For each iteration apply:
 *         For each key-value pair (k,v) in the current map:
 *            T[v] := k + (T[v] if defined else 0)
 *         (All updates are based on the values before the iteration.)
 *
 *  a(n) is the number of entries in T after n iterations
 *  (a(0)=1).  This implementation directly simulates the
 *  process using open‑addressing hash tables.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t *keys;   /* keys (0 means empty) */
    seq_t *vals;   /* values */
    size_t *inds;  /* indices of filled slots */
    size_t cap;    /* power of two table size */
    size_t filled; /* number of entries */
} HashTable;

/* Create a new hash table with given capacity (must be power of two). */
static void htable_init(HashTable *t, size_t cap) {
    t->cap = cap;
    t->keys = (seq_t *)calloc(cap, sizeof(seq_t));
    t->vals = (seq_t *)calloc(cap, sizeof(seq_t));
    t->inds = (size_t *)malloc(cap * sizeof(size_t));
    t->filled = 0;
}

/* Free memory owned by the table. */
static void htable_free(HashTable *t) {
    free(t->keys);
    free(t->vals);
    free(t->inds);
}

/* Clear all entries while preserving capacity. */
static void htable_clear(HashTable *t) {
    for (size_t i = 0; i < t->filled; ++i)
        t->keys[t->inds[i]] = 0;
    t->filled = 0;
}

/* Insert a key with value addition into the table. */
static void htable_insert_add(HashTable *t, seq_t key, seq_t add_val) {
    /* Murmur3 style hash (64‑bit mix). */
    uint64_t k64 = (uint64_t)key;
    uint64_t hash = k64 * 11400714819323198485ULL;
    size_t idx = (size_t)hash & (t->cap - 1);

    while (t->keys[idx]) {
        if (t->keys[idx] == key) {
            t->vals[idx] += add_val;
            return;
        }
        idx = (idx + 1) & (t->cap - 1);
    }
    /* new entry */
    t->keys[idx] = key;
    t->vals[idx] = add_val;
    t->inds[t->filled++] = idx;
}

/* Compute next power of two >= v. */
static size_t next_pow2(size_t v) {
    size_t p = 1;
    while (p < v) p <<= 1;
    return p;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* At n = 0 the initial map has one entry. */
    if (n == 0) return (seq_t)1;

    /* Determine a safe initial capacity (enough for 10000 iterations). */
    const size_t INIT_CAP = 1u << 20; /* 1,048,576 slots */

    HashTable t1, t2;
    htable_init(&t1, INIT_CAP);
    htable_init(&t2, INIT_CAP);

    /* initial map: T[1] = 1 */
    htable_insert_add(&t1, (seq_t)1, (seq_t)1);

    for (idx_t iter = 1; iter <= n; ++iter) {
        /* Clear destination table (t2). */
        htable_clear(&t2);

        /* For every entry in t1, apply the two updates. */
        for (size_t i = 0; i < t1.filled; ++i) {
            seq_t k   = t1.keys[t1.inds[i]];
            seq_t val = t1.vals[t1.inds[i]];   /* old value T[k] */
            /* First update: T[val] += k. */
            htable_insert_add(&t2, val, k);
            /* Second update: T[k] += val. */
            htable_insert_add(&t2, k, val);
        }

        /* Swap tables: t1 becomes the new current map. */
        HashTable tmp = t1;
        t1 = t2;
        t2 = tmp;
    }

    seq_t result = (seq_t)t1.filled;

    htable_free(&t1);
    htable_free(&t2);
    return result;
}
