/*
 * A336037  a(n+1) is the number of times the binary representation of a(n) is
 *          the concatenation of that of one or more consecutive terms in this
 *          sequence so far with a(1) = 1.
 * https://oeis.org/A336037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336037 */
/* a(1) = 1, a(n+1) = number of times the binary representation of a(n)
   is the concatenation of one or more consecutive terms among a(1)..a(n) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* ---------- hash table for (value → count) ---------- */
#define HASH_CAP (1 << 20)           /* 1,048,576 entries, load factor < 0.5 for 10 000 terms */
#define MAX_BITS (sizeof(seq_t) * 8)

typedef struct {
    seq_t key;
    seq_t count;
    unsigned char occupied;
} HashEntry;

static HashEntry hashTable[HASH_CAP];
static size_t hash_cnt = 0;

/* simple 64‑bit mix derived from splitmix64 */
static inline uint64_t mix64(uint64_t z) {
    z += 0x9e3779b97f4a7c15ULL;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

/* hash a seq_t value to uint64_t */
static inline uint64_t hash_key(seq_t x) {
    uint64_t low, high;
    if (sizeof(seq_t) <= 8) {
        low  = (uint64_t)x;
        high = 0;
    } else {
        low  = (uint64_t)x;
        high = (uint64_t)((x >> 64) & (((seq_t)1 << 64) - 1));
    }
    return mix64(low ^ high);
}

/* increment count of a key, inserting if absent */
static void hash_inc(seq_t key) {
    if (hash_cnt * 2 >= HASH_CAP) return;  /* capacity full – do nothing (very unlikely) */
    size_t idx = hash_key(key) & (HASH_CAP - 1);
    for (;;) {
        if (!hashTable[idx].occupied) {
            hashTable[idx].occupied = 1;
            hashTable[idx].key = key;
            hashTable[idx].count = 1;
            ++hash_cnt;
            return;
        }
        if (hashTable[idx].key == key) {
            ++hashTable[idx].count;
            return;
        }
        idx = (idx + 1) & (HASH_CAP - 1);
    }
}

/* get count of a key, or 0 if absent */
static inline seq_t hash_get(seq_t key) {
    size_t idx = hash_key(key) & (HASH_CAP - 1);
    for (;;) {
        if (!hashTable[idx].occupied) return 0;
        if (hashTable[idx].key == key) return hashTable[idx].count;
        idx = (idx + 1) & (HASH_CAP - 1);
    }
}

/* ---------- subarray storage ---------- */
#define MAX_N 20000   /* larger than the required 10 000 */

typedef struct {
    seq_t val;
    unsigned len;     /* bit length of val */
} SubEntry;

static SubEntry sub_buf[2][MAX_N];
static SubEntry *prev_sub = sub_buf[0];
static SubEntry *next_sub = sub_buf[1];
static size_t prev_len = 0;
static size_t next_len = 0;

/* ---------- sequence storage ---------- */
static seq_t seq_arr[MAX_N];
static size_t seq_len = 0;

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 */

    size_t target = (size_t)n;     /* n fits in size_t for current use */

    /* generate up to target */
    while (seq_len < target) {
        if (seq_len == 0) {        /* a(1) = 1 */
            seq_arr[0] = (seq_t)1;
            prev_len = 1;
            prev_sub[0].val = seq_arr[0];
            prev_sub[0].len = bitlen_seq_t(seq_arr[0]);

            /* record own subarray */
            hash_inc(seq_arr[0]);

            ++seq_len;
            continue;
        }

        /* compute a(n+1) as count of previous term */
        seq_t last = seq_arr[seq_len - 1];
        seq_t next_val = hash_get(last);

        seq_arr[seq_len] = next_val;
        ++seq_len;

        unsigned cur_len = bitlen_seq_t(next_val);

        /* reset next buffer */
        next_len = 0;

        /* own subarray: [a(n)] */
        next_sub[next_len].val = next_val;
        next_sub[next_len].len = cur_len;
        hash_inc(next_val);
        ++next_len;

        /* extend all subarrays ending at previous term */
        for (size_t i = 0; i < prev_len; ++i) {
            unsigned prev_len_val = prev_sub[i].len;
            if (prev_len_val + cur_len >= MAX_BITS) continue;  /* avoid overflow */

            seq_t val = (prev_sub[i].val << cur_len) | next_val;
            unsigned new_len = prev_len_val + cur_len;

            next_sub[next_len].val = val;
            next_sub[next_len].len = new_len;
            hash_inc(val);
            ++next_len;
        }

        /* swap buffers for next iteration */
        SubEntry *tmp = prev_sub;
        prev_sub = next_sub;
        next_sub = tmp;
        prev_len = next_len;
    }

    return seq_arr[n - 1];
}
