/*
 * A324663  Starting at n, a(n) is the number of moves made away from zero
 *          according to the following rules. On the k-th step (k=1,2,3,...)
 *          move a distance of k in the direction of zero. If the number
 *          landed on has been landed on before, move a distance of k away
 *          from zero instead.
 * https://oeis.org/A324663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulate the deterministic walk for A324663; count steps taken away
   from zero during the walk. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Simple open‑addressing hash set for int64_t keys. */
typedef struct {
    size_t capacity;
    size_t mask;
    size_t size;
    int64_t *table;
    int64_t EMPTY;
} HashSet;

static size_t hash_int64(int64_t x) {
    return (size_t)((uint64_t)x * 11400714819323198485ull);
}

static void hs_init(HashSet *hs, size_t capacity) {
    hs->capacity = capacity;
    hs->mask = capacity - 1;
    hs->size = 0;
    hs->table = (int64_t *)calloc(capacity, sizeof(int64_t));
    if (!hs->table) abort();
    hs->EMPTY = LLONG_MAX;
    for (size_t i = 0; i < capacity; ++i) hs->table[i] = hs->EMPTY;
}

static void hs_free(HashSet *hs) {
    free(hs->table);
}

static void hs_resize(HashSet *hs, size_t new_capacity) {
    int64_t *old = hs->table;
    size_t old_cap = hs->capacity;

    hs->capacity = new_capacity;
    hs->mask = new_capacity - 1;
    hs->table = (int64_t *)calloc(new_capacity, sizeof(int64_t));
    if (!hs->table) abort();
    for (size_t i = 0; i < new_capacity; ++i) hs->table[i] = hs->EMPTY;

    for (size_t i = 0; i < old_cap; ++i) {
        int64_t key = old[i];
        if (key != hs->EMPTY) {
            size_t idx = hash_int64(key) & hs->mask;
            while (hs->table[idx] != hs->EMPTY) idx = (idx + 1) & hs->mask;
            hs->table[idx] = key;
        }
    }
    free(old);
}

static void hs_insert(HashSet *hs, int64_t key) {
    if (hs->size * 2 >= hs->capacity) hs_resize(hs, hs->capacity << 1);
    size_t idx = hash_int64(key) & hs->mask;
    while (hs->table[idx] != hs->EMPTY) {
        if (hs->table[idx] == key) return;
        idx = (idx + 1) & hs->mask;
    }
    hs->table[idx] = key;
    hs->size++;
}

static bool hs_contains(const HashSet *hs, int64_t key) {
    size_t idx = hash_int64(key) & hs->mask;
    while (hs->table[idx] != hs->EMPTY) {
        if (hs->table[idx] == key) return true;
        idx = (idx + 1) & hs->mask;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    int64_t pos = (int64_t)n;
    size_t step = 1;
    seq_t away = 0;

    HashSet hs;
    hs_init(&hs, 64);
    hs_insert(&hs, pos);

    while (1) {
        int64_t target;
        if (pos > 0) {
            target = pos - (int64_t)step;
        } else {
            target = pos + (int64_t)step;
        }

        if (hs_contains(&hs, target)) {
            /* move away instead */
            if (pos > 0) {
                target = pos + (int64_t)step;
            } else {
                target = pos - (int64_t)step;
            }
            away++;
        }

        pos = target;
        hs_insert(&hs, pos);

        if (pos == 0) break;
        step++;
    }

    hs_free(&hs);
    return away;
}
