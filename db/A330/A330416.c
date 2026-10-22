/*
 * A330416  a(n) = (number of times a(n-2) has already appeared) + (number of
 *          times a(n-3) has already appeared), starting with a(n) = n for
 *          n<3.
 * https://oeis.org/A330416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of times a(n-2) has already appeared) * + (number
 *            of times a(n-3) has already appeared) * Initial values: a(0)=0,
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---- internal dynamic storage ---- */
static seq_t *seq      = NULL;        /* sequence array */
static size_t seq_cap  = 0;           /* capacity of seq   */
static size_t seq_len  = 0;           /* number of elements in seq */

static uint64_t *ht_keys   = NULL;    /* hash table keys   */
static unsigned long long *ht_counts = NULL;  /* hash table counts */
static unsigned char *ht_used  = NULL;/* occupancy flag    */
static size_t ht_cap   = 0;   /* size of hash table (power of two) */
static size_t ht_mask  = 0;   /* mask for indexing */
static size_t ht_len   = 0;   /* number of used entries */

/* ---- hash table helpers ---- */
static void rehash_hash(void)
{
    size_t new_cap = ht_cap << 1;
    uint64_t *new_keys = (uint64_t *)malloc(sizeof(uint64_t) * new_cap);
    unsigned long long *new_counts = (unsigned long long *)malloc(sizeof(unsigned long long) * new_cap);
    unsigned char *new_used = (unsigned char *)calloc(new_cap, 1);
    size_t new_mask = new_cap - 1;

    for (size_t i = 0; i < ht_cap; ++i) {
        if (ht_used[i]) {
            uint64_t k = ht_keys[i];
            unsigned long long cnt = ht_counts[i];

            size_t idx = ((size_t)k) & new_mask;
            while (new_used[idx]) {
                idx = (idx + 1) & new_mask;
            }
            new_used[idx]        = 1;
            new_keys[idx]        = k;
            new_counts[idx]      = cnt;
        }
    }

    free(ht_keys);
    free(ht_counts);
    free(ht_used);

    ht_keys  = new_keys;
    ht_counts= new_counts;
    ht_used  = new_used;
    ht_cap   = new_cap;
    ht_mask  = new_mask;
}

static void add_value(seq_t v)
{
    uint64_t k = (uint64_t)v;
    size_t idx = ((size_t)k) & ht_mask;

    while (ht_used[idx]) {
        if (ht_keys[idx] == k) {
            ht_counts[idx]++;        /* found, increment count */
            return;
        }
        idx = (idx + 1) & ht_mask;
    }

    /* insert new entry */
    ht_used[idx]   = 1;
    ht_keys[idx]   = k;
    ht_counts[idx] = 1;
    ht_len++;

    /* rehash if load factor > 0.7 */
    if ((ht_len * 10) > (ht_cap * 7)) {
        rehash_hash();
    }
}

static unsigned long long get_count(seq_t v)
{
    uint64_t k = (uint64_t)v;
    size_t idx = ((size_t)k) & ht_mask;

    while (ht_used[idx]) {
        if (ht_keys[idx] == k)
            return ht_counts[idx];
        idx = (idx + 1) & ht_mask;
    }
    return 0;
}

/* ---- ensure data for index <= n ---- */
static void ensure_sequence(idx_t n)
{
    size_t target = (size_t)n;

    /* lazy initialization of sequence array */
    if (seq == NULL) {
        seq_cap = 16;
        seq     = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq_len = 0;
    }

    /* lazy initialization of hash table */
    if (ht_keys == NULL) {
        ht_cap  = 16;
        ht_mask = ht_cap - 1;
        ht_keys  = (uint64_t *)malloc(sizeof(uint64_t) * ht_cap);
        ht_counts= (unsigned long long *)malloc(sizeof(unsigned long long) * ht_cap);
        ht_used  = (unsigned char *)calloc(ht_cap, 1);
        ht_len   = 0;
    }

    for (; seq_len <= target; ++seq_len) {
        /* grow sequence array if necessary */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq = (seq_t *)realloc(seq, sizeof(seq_t) * new_cap);
            seq_cap = new_cap;
        }

        if (seq_len < 3) {
            seq_t val = (seq_t)seq_len;
            seq[seq_len] = val;
            add_value(val);
        } else {
            seq_t v1 = seq[seq_len - 2];
            seq_t v2 = seq[seq_len - 3];
            unsigned long long c1 = get_count(v1);
            unsigned long long c2 = get_count(v2);
            seq_t val = (seq_t)(c1 + c2);
            seq[seq_len] = val;
            add_value(val);
        }
    }
}

/* ---- required sequence function ---- */
seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq[(size_t)n];
}
