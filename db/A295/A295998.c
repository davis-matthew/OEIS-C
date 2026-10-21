/*
 * A295998  Solution of the complementary equation a(n) = 2*a(n-2) + b(n-2),
 *          where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + b(n-2), a(0)=1, a(1)=2, b(0)=3; a and b are
 *            complementary increasing sequences. This implementation memoises
 *            computed values and keeps a hash set of all used numbers for O(n
 *            log n) time
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 58, u64 n <= 122,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* -------------------------------- hash set for seq_t -------------------------------- */
static size_t used_table_size = 0;          /* power of two */
static seq_t *used_keys = NULL;
static unsigned char *used_occupied = NULL;
static size_t used_used = 0;

/* hash function: low 64 bits of the value */
static inline size_t seq_hash(seq_t x) {
    return (size_t)( (unsigned long long)x );
}

/* initialise hash set large enough for `max_items` entries (load < 0.75) */
static void used_init(size_t max_items) {
    size_t cap = 8;
    while (cap <= max_items * 4) cap <<= 1;           /* at least 4x load factor */
    used_table_size = cap;
    used_keys = (seq_t *)malloc(cap * sizeof(seq_t));
    used_occupied = (unsigned char *)malloc(cap * sizeof(unsigned char));
    if (!used_keys || !used_occupied) abort();
    memset(used_occupied, 0, cap);
    used_used = 0;
}

/* rehash to twice the size */
static void used_rehash(void) {
    size_t old_size = used_table_size;
    size_t new_size = old_size << 1;
    seq_t *new_keys = (seq_t *)malloc(new_size * sizeof(seq_t));
    unsigned char *new_occ = (unsigned char *)malloc(new_size * sizeof(unsigned char));
    if (!new_keys || !new_occ) abort();
    memset(new_occ, 0, new_size);
    /* reinsert all keys */
    for (size_t i = 0; i < old_size; ++i) {
        if (used_occupied[i]) {
            seq_t k = used_keys[i];
            size_t h = seq_hash(k) & (new_size - 1);
            while (new_occ[h]) h = (h + 1) & (new_size - 1);
            new_keys[h] = k;
            new_occ[h] = 1;
        }
    }
    free(used_keys);
    free(used_occupied);
    used_keys = new_keys;
    used_occupied = new_occ;
    used_table_size = new_size;
}

/* insert key into hash set */
static void used_insert(seq_t key) {
    if (used_used * 3 >= used_table_size * 2) used_rehash();
    size_t h = seq_hash(key) & (used_table_size - 1);
    while (used_occupied[h]) {
        if (used_keys[h] == key) return;   /* already present */
        h = (h + 1) & (used_table_size - 1);
    }
    used_keys[h] = key;
    used_occupied[h] = 1;
    ++used_used;
}

/* check presence */
static bool used_contains(seq_t key) {
    size_t h = seq_hash(key) & (used_table_size - 1);
    while (used_occupied[h]) {
        if (used_keys[h] == key) return true;
        h = (h + 1) & (used_table_size - 1);
    }
    return false;
}

/* -------------------------------- sequence storage -------------------------------- */
static seq_t *a_arr = NULL;
static seq_t *b_arr = NULL;
static size_t a_cap = 0;
static idx_t len = 0;          /* number of stored terms */

/* ensure array capacity for at least `needed` indices (0‑based) */
static void a_ensure_capacity(idx_t needed) {
    if (a_cap > (size_t)needed) return;
    size_t new_cap = a_cap ? a_cap * 2 : 4;
    while (new_cap <= (size_t)needed) new_cap <<= 1;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    if (!a_arr || !b_arr) abort();
    a_cap = new_cap;
}

/* public function */
seq_t a(idx_t n) {
    /* first call: initialise */
    if (len == 0) {
        a_ensure_capacity(1);
        a_arr[0] = (seq_t)1;
        b_arr[0] = (seq_t)3;
        len = 1;
        used_init(4);
        used_insert(a_arr[0]);
        used_insert(b_arr[0]);
    }

    /* compute up to n if necessary */
    while (len <= n) {
        idx_t i = len;          /* next index to compute */
        a_ensure_capacity(i);
        if (i == 1) {
            a_arr[1] = (seq_t)2;
            used_insert(a_arr[1]);
            seq_t cand = b_arr[0] + (seq_t)1;
            while (used_contains(cand)) cand++;
            b_arr[1] = cand;
            used_insert(b_arr[1]);
        } else {
            a_arr[i] = ((seq_t)2 * a_arr[i - 2]) + b_arr[i - 2];
            used_insert(a_arr[i]);
            seq_t cand = b_arr[i - 1] + (seq_t)1;
            while (used_contains(cand)) cand++;
            b_arr[i] = cand;
            used_insert(b_arr[i]);
        }
        ++len;
    }
    return a_arr[n];
}
