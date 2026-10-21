/*
 * A257340  Arrange numbers in a single clockwise spiral so that each number
 *          is relatively prime to its four (N,S,E,W) neighbors.
 * https://oeis.org/A257340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by constructing a clockwise spiral; at each step
 *            the smallest unused positive integer coprime to all
 *            already-placed orthogonal neighbors is placed
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, key_from_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/key_from_coords.h"

/* Internal data structures and static state */
static seq_t *sequence = NULL;       /* the generated sequence */
static size_t seq_cap = 0;           /* capacity of sequence array */
static size_t seq_len = 0;           /* number of values computed */

static bool *used = NULL;            /* used[i] == true if i already placed */
static size_t used_cap = 0;          /* capacity of used array */
static seq_t min_unused = 2;         /* smallest integer not yet used */

typedef struct {
    uint64_t key;
    seq_t   value;
    bool    occupied;
} Entry;

static Entry *hash_table = NULL;     /* hash map from (x,y) → value */
static size_t hash_cap = 1 << 18;    /* 262144 entries, power of two */

static inline size_t hash_index(uint64_t k)
{
    return (size_t)((k * 11400714819323198485ULL) & (hash_cap - 1));
}
static void hash_insert(int32_t x, int32_t y, seq_t val)
{
    uint64_t k = key_from_coords(x, y);
    size_t idx = hash_index(k);
    while (hash_table[idx].occupied) {
        if (hash_table[idx].key == k) {
            hash_table[idx].value = val;
            return;
        }
        idx = (idx + 1) & (hash_cap - 1);
    }
    hash_table[idx].occupied = true;
    hash_table[idx].key = k;
    hash_table[idx].value = val;
}
static seq_t hash_find(int32_t x, int32_t y)
{
    uint64_t k = key_from_coords(x, y);
    size_t idx = hash_index(k);
    while (hash_table[idx].occupied) {
        if (hash_table[idx].key == k)
            return hash_table[idx].value;
        idx = (idx + 1) & (hash_cap - 1);
    }
    return 0;   /* no entry */
}

/* Spiral traversal state */
static int32_t cx = 0, cy = 0;       /* current coordinates */
static int32_t dx = 1, dy = 0;       /* current direction (right) */
static int32_t step_size = 1;        /* number of steps in current leg */
static int32_t steps_in_dir = 0;     /* steps taken in current direction */
static int32_t turns = 0;            /* how many turns made */

/* The exported function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* not defined for offset 0 */

    /* Lazy initialization of the data structures */
    if (seq_len == 0) {
        /* initial capacity for the sequence */
        seq_cap = 32;
        sequence = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        /* used array: small initial capacity */
        used_cap = 64;
        used = (bool *)calloc(used_cap, sizeof(bool));

        /* hash table */
        hash_table = (Entry *)calloc(hash_cap, sizeof(Entry));

        /* place the first number at origin (0,0) */
        sequence[0] = (seq_t)1;
        seq_len = 1;
        used[1] = true;
        hash_insert(0, 0, sequence[0]);

        /* spiral state already initialized above */
    }

    /* Generate further terms until the required index is reached */
    while (seq_len < n) {
        /* Advance to the next coordinate in the clockwise spiral */
        cx += dx;
        cy += dy;
        steps_in_dir++;
        if (steps_in_dir == step_size) {
            steps_in_dir = 0;
            turns++;
            /* rotate right: (dx,dy) → (-dy,dx) */
            int32_t t = dx;
            dx = -dy;
            dy = t;
            if (turns % 2 == 0) step_size++;
        }

        /* Find the smallest unused integer that is coprime to all neighbors */
        seq_t candidate = min_unused;
        for (;;) {
            size_t ci = (size_t)candidate;
            if (ci >= used_cap) {
                size_t new_cap = used_cap * 2;
                if (new_cap <= ci) new_cap = ci + 1;
                bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
                if (!new_used) abort();
                for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
                used = new_used;
                used_cap = new_cap;
            }
            if (!used[ci]) {
                bool ok = true;
                seq_t v;

                v = hash_find(cx, cy + 1);  /* North */
                if (v && gcd(candidate, v) != 1) ok = false;

                if (ok) {
                    v = hash_find(cx, cy - 1);  /* South */
                    if (v && gcd(candidate, v) != 1) ok = false;
                }
                if (ok) {
                    v = hash_find(cx + 1, cy);  /* East */
                    if (v && gcd(candidate, v) != 1) ok = false;
                }
                if (ok) {
                    v = hash_find(cx - 1, cy);  /* West */
                    if (v && gcd(candidate, v) != 1) ok = false;
                }
                if (ok) break;   /* candidate accepted */
            }
            ++candidate;
        }

        /* Ensure capacity in the sequence array */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_t *new_seq = (seq_t *)realloc(sequence, seq_cap * sizeof(seq_t));
            if (!new_seq) abort();
            sequence = new_seq;
        }
        sequence[seq_len++] = candidate;

        /* Mark this number as used */
        used[(size_t)candidate] = true;

        /* Record its position in the hash table */
        hash_insert(cx, cy, candidate);

        /* Update min_unused to the next unused integer */
        size_t mi = (size_t)min_unused;
        while (mi < used_cap && used[mi]) ++mi;
        min_unused = (seq_t)mi;
        if (mi >= used_cap) {
            size_t new_cap = used_cap * 2;
            if (new_cap <= mi) new_cap = mi + 1;
            bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
            if (!new_used) abort();
            for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
            used = new_used;
            used_cap = new_cap;
        }
    }

    return sequence[(size_t)(n - 1)];
}
