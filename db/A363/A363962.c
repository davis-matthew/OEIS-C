/*
 * A363962  a(1)=1; for n > 1, if n appears in the sequence then a(n) =
 *          lastindex(n), where lastindex(n) is the index of the last
 *          appearance of n. Otherwise a(n) = a(n-1) - n unless that result is
 *          already in the sequence or would be negative; otherwise, a(n) =
 *          a(n-1) * n.
 * https://oeis.org/A363962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, if n appears in the sequence then
 *            a(n)=lastindex(n), otherwise a(n)=a(n-1)-n when that value is
 *            non‑negative and not already in the sequence; otherwise
 *            a(n)=a(n-1)*n
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Persistent storage for the sequence and a hash table that maps a value
   to its most recent index (the “last index” in the definition). */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

typedef struct {
    seq_t key;
    idx_t val;
    bool used;
} Entry;

static Entry *hash = NULL;
static size_t hash_cap = 0;
static size_t hash_used = 0;

/* Small helpers to grow the dynamic arrays. */
static void seq_grow(size_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void hash_rehash(void) {
    size_t new_cap = hash_cap ? hash_cap * 2 : 16;
    Entry *new_hash = (Entry *)calloc(new_cap, sizeof(Entry));
    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash[i].used) {
            uint64_t h = (uint64_t)hash[i].key;
            size_t idx = h & (new_cap - 1);
            while (new_hash[idx].used) idx = (idx + 1) & (new_cap - 1);
            new_hash[idx].key  = hash[i].key;
            new_hash[idx].val  = hash[i].val;
            new_hash[idx].used = true;
        }
    }
    free(hash);
    hash = new_hash;
    hash_cap = new_cap;
    hash_used = 0;
    for (size_t i = 0; i < hash_cap; ++i)
        if (hash[i].used) ++hash_used;
}

static void hash_set(seq_t key, idx_t val) {
    if (hash_cap == 0 || (hash_used * 3 > hash_cap * 2))
        hash_rehash();
    uint64_t h = (uint64_t)key;
    size_t idx = h & (hash_cap - 1);
    while (hash[idx].used && hash[idx].key != key) {
        idx = (idx + 1) & (hash_cap - 1);
    }
    if (!hash[idx].used) ++hash_used;
    hash[idx].key  = key;
    hash[idx].val  = val;
    hash[idx].used = true;
}

static bool hash_get(seq_t key, idx_t *out) {
    if (hash_cap == 0) return false;
    uint64_t h = (uint64_t)key;
    size_t idx = h & (hash_cap - 1);
    while (hash[idx].used) {
        if (hash[idx].key == key) {
            if (out) *out = hash[idx].val;
            return true;
        }
        idx = (idx + 1) & (hash_cap - 1);
    }
    return false;
}

/* Main sequence generator function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not part of the sequence, but safe */
    /* Ensure that a(1) is initialised. */
    if (seq_len == 0) {
        seq_grow(1);
        seq[0]  = (seq_t)1;
        hash_set((seq_t)1, (idx_t)1);
        seq_len = 1;
    }
    while (seq_len < n) {
        idx_t cur_idx = (idx_t)(seq_len + 1); /* next index (1‑based) */
        seq_grow(seq_len + 1);
        seq_t prev = seq[seq_len - 1];
        idx_t last_pos;
        /* Rule: if n has appeared, use its last index. */
        if (hash_get((seq_t)cur_idx, &last_pos)) {
            seq[seq_len] = last_pos;
        } else {
            /* Try subtraction if possible and the result is new. */
            if (prev >= (seq_t)cur_idx) {
                seq_t cand = prev - (seq_t)cur_idx;
                idx_t dummy;
                if (!hash_get(cand, &dummy)) {
                    seq[seq_len] = cand;
                } else {
                    seq[seq_len] = prev * (seq_t)cur_idx;
                }
            } else {
                /* Subtraction would be negative – fall back to multiplication. */
                seq[seq_len] = prev * (seq_t)cur_idx;
            }
        }
        /* Record the new value and its index. */
        hash_set(seq[seq_len], cur_idx);
        ++seq_len;
    }
    return seq[n - 1];
}
