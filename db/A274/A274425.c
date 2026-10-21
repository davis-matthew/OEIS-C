/*
 * A274425  Consider van Eck's sequences with a(1)=n and a(2)=0. Sequence
 *          lists the index of second occurrence of n.
 * https://oeis.org/A274425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of the second occurrence of n in the van Eck
 *            sequence with a(1)=n, a(2)=0. The value is computed by direct
 *            simulation of the recurrence: a(1)=n, a(2)=0 for k>2: if a(k‑1)
 *            has occurred before, a(k) = (k‑1) – last_occurrence(a(k‑1)) else
 *            a(k) = 0 The function stops when a(k)=n for the...
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
/* ---------- C++ implementation (uses std::map) ---------- */
#include <map>
#include <utility>

using MapType = std::map<seq_t, idx_t>;

seq_t a(idx_t n) {
    if (n == 0) return 2;                 /* undefined case, but guard */

    seq_t start = (seq_t)n;
    MapType last;

    last[start] = 1;        // index of first occurrence
    last[(seq_t)0] = 2;     // a(2) = 0

    seq_t prev = (seq_t)0;   // a(2)
    for (idx_t k = 3; ; ++k) {
        idx_t last_occ = 0;
        auto it = last.find(prev);
        bool found = (it != last.end());
        if (found) last_occ = it->second;

        seq_t next = found ? (seq_t)((k - 1) - last_occ) : (seq_t)0;

        last[prev] = k - 1;    // update last occurrence of prev

        if (next == start) return k;

        prev = next;
    }
}

#else
/* ---------- C implementation (simple open‑address hash table) ---------- */

#include <string.h>

/* Hash table for mapping seq_t → idx_t (last occurrence) */
struct MapEntry {
    bool occupied;
    seq_t key;
    idx_t value;
};

struct Map {
    struct MapEntry *entries;
    size_t capacity;  /* power of two */
    size_t count;
};

static inline size_t hash_seq_t(seq_t v) {
    size_t h = 2166136261u;
    const unsigned char *p = (const unsigned char *)&v;
    for (size_t i = 0; i < sizeof(seq_t); ++i) {
        h ^= p[i];
        h *= 16777619u;
    }
    return h;
}

static void map_init(struct Map *m, size_t min_cap) {
    size_t cap = 1;
    while (cap < min_cap) cap <<= 1;
    m->entries = (struct MapEntry *)malloc(cap * sizeof(struct MapEntry));
    for (size_t i = 0; i < cap; ++i) m->entries[i].occupied = false;
    m->capacity = cap;
    m->count = 0;
}

static void map_free(struct Map *m) {
    free(m->entries);
    m->entries = NULL;
    m->capacity = 0;
    m->count = 0;
}

static void map_rehash(struct Map *m) {
    size_t old_cap = m->capacity;
    struct MapEntry *old_entries = m->entries;

    size_t new_cap = old_cap << 1;
    struct MapEntry *new_entries = (struct MapEntry *)malloc(new_cap * sizeof(struct MapEntry));
    for (size_t i = 0; i < new_cap; ++i) new_entries[i].occupied = false;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old_entries[i].occupied) {
            size_t idx = hash_seq_t(old_entries[i].key) & (new_cap - 1);
            while (new_entries[idx].occupied) idx = (idx + 1) & (new_cap - 1);
            new_entries[idx] = old_entries[i];
        }
    }
    free(old_entries);
    m->entries = new_entries;
    m->capacity = new_cap;
}

static bool map_get(const struct Map *m, seq_t key, idx_t *out) {
    size_t idx = hash_seq_t(key) & (m->capacity - 1);
    while (m->entries[idx].occupied) {
        if (m->entries[idx].key == key) {
            *out = m->entries[idx].value;
            return true;
        }
        idx = (idx + 1) & (m->capacity - 1);
    }
    return false;
}

static void map_set(struct Map *m, seq_t key, idx_t value) {
    if (m->count * 3 >= m->capacity * 2) map_rehash(m);

    size_t idx = hash_seq_t(key) & (m->capacity - 1);
    while (m->entries[idx].occupied) {
        if (m->entries[idx].key == key) {
            m->entries[idx].value = value;
            return;
        }
        idx = (idx + 1) & (m->capacity - 1);
    }
    m->entries[idx].occupied = true;
    m->entries[idx].key = key;
    m->entries[idx].value = value;
    m->count++;
}

seq_t a(idx_t n) {
    if (n == 0) return 2;                 /* guard, though n>=1 by spec */

    seq_t start = (seq_t)n;
    struct Map *m = (struct Map *)malloc(sizeof(struct Map));
    map_init(m, 64);                      // start with 64 slots

    map_set(m, start, 1);                 // a(1) = n
    map_set(m, (seq_t)0, 2);              // a(2) = 0

    seq_t prev = (seq_t)0;                // current term at index 2
    for (idx_t k = 3; ; ++k) {
        idx_t last_occ = 0;
        bool found = map_get(m, prev, &last_occ);
        seq_t next = found ? (seq_t)((k - 1) - last_occ) : (seq_t)0;

        map_set(m, prev, k - 1);          // update last occurrence of prev

        if (next == start) {
            map_free(m);
            free(m);
            return k;                    // second occurrence index
        }

        prev = next;
    }
}
#endif
