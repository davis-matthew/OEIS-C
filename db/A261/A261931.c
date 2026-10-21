/*
 * A261931  a(0)=1, a(1)=2, for n>=1, a(2*n) = 6*n - 2; a(2*n+1) =
 *          min(((a(2*n)-1)/3)^, (2*a(2*n))^, (2*a(2*n-1))^), where, instead
 *          of t, we write t^, if t has not appeared earlier in the sequence
 *          and is neither of the form 3*k nor of the form 6*m-2.
 * https://oeis.org/A261931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261931
 *   a(0)=1, a(1)=2
 *   For n>=1:
 *      a(2*n)   = 6*n-2
 *      a(2*n+1) = min{ (a(2*n)-1)/3 , 2*a(2*n) , 2*a(2*n-1) }
 *   Candidate values are accepted only if they have not appeared
 *   before, are not divisible by 3, and are not congruent to 4 (mod 6).
 *   The sequence is a permutation of the positive integers not divisible
 *   by 3 which are not of the form 12*s+8 (s≥2).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- local helpers ---------- */
static inline bool is_forbidden(seq_t v) {
    /* divisible by 3 or congruent to 4 mod 6 */
    return (v % 3 == 0) || (v % 6 == 4);
}

/* ---------- global state ---------- */
static seq_t *seq_arr = NULL;      /* computed terms */
static size_t seq_len = 0;         /* number of terms computed */
static size_t seq_cap = 0;         /* allocated capacity */

static const bool use_hash = (sizeof(seq_t) <= 8);

/* hash table for 64‑bit values */
#if use_hash
static uint64_t *hash_tbl = NULL;
static size_t hash_size = 0;   /* power of two, >0 */
static size_t hash_used = 0;   /* number of occupied slots */

/* hash function for 64‑bit values */
static inline uint64_t hash_uint64(uint64_t v) {
    v ^= v >> 33;
    v *= 0xff51afd7ed558ccdULL;
    v ^= v >> 33;
    v *= 0xc4ceb9fe1a85ec53ULL;
    v ^= v >> 33;
    return v;
}

static void resize_hash(void) {
    size_t old_size = hash_size;
    size_t new_size = old_size << 1;
    uint64_t *new_tbl = (uint64_t *)calloc(new_size, sizeof(uint64_t));
    if (!new_tbl) abort();
    size_t mask = new_size - 1;
    for (size_t i = 0; i < old_size; ++i) {
        uint64_t v = hash_tbl[i];
        if (v == 0) continue;
        for (size_t j = 0; j < new_size; ++j) {
            size_t idx = (hash_uint64(v) + j) & mask;
            if (new_tbl[idx] == 0) {
                new_tbl[idx] = v;
                break;
            }
        }
    }
    free(hash_tbl);
    hash_tbl = new_tbl;
    hash_size = new_size;
}

static inline bool hash_contains(uint64_t v) {
    size_t mask = hash_size - 1;
    for (size_t i = 0; i < hash_size; ++i) {
        size_t idx = (hash_uint64(v) + i) & mask;
        uint64_t val = hash_tbl[idx];
        if (val == 0) return false;
        if (val == v) return true;
    }
    return false;
}

static inline void hash_add(uint64_t v) {
    size_t mask = hash_size - 1;
    for (size_t i = 0; i < hash_size; ++i) {
        size_t idx = (hash_uint64(v) + i) & mask;
        uint64_t val = hash_tbl[idx];
        if (val == 0) {
            hash_tbl[idx] = v;
            ++hash_used;
            if (hash_used * 4 >= hash_size * 3)
                resize_hash();
            return;
        }
        if (val == v) return;
    }
}
#endif

/* linear search set for big values */
#if !use_hash
static seq_t *big_set = NULL;
static size_t big_len = 0;
static size_t big_cap = 0;

static inline bool big_contains(seq_t v) {
    for (size_t i = 0; i < big_len; ++i)
        if (big_set[i] == v) return true;
    return false;
}

static inline void big_add(seq_t v) {
    if (big_len == big_cap) {
        size_t new_cap = big_cap ? big_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(big_set, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        big_set = tmp;
        big_cap = new_cap;
    }
    big_set[big_len++] = v;
}
#endif

/* ---------------- helpers for inside seq_t type ---------- */
static inline bool val_seen(seq_t v) {
#if use_hash
    return hash_contains((uint64_t)v);
#else
    return big_contains(v);
#endif
}

static inline void val_add(seq_t v) {
#if use_hash
    hash_add((uint64_t)v);
#else
    big_add(v);
#endif
}

/* ---------- initialization ---------- */
static void init(void) {
    seq_arr = NULL;
    seq_len = 0;
    seq_cap = 0;
#if use_hash
    hash_tbl = (uint64_t *)calloc(8, sizeof(uint64_t));
    if (!hash_tbl) abort();
    hash_size = 8;
    hash_used = 0;
#else
    big_set = (seq_t *)calloc(16, sizeof(seq_t));
    if (!big_set) abort();
    big_cap = 16;
    big_len = 0;
#endif
}

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) { init(); initialized = true; }

    if ((idx_t)seq_len <= n) {
        size_t old_len = seq_len;
        size_t needed = (size_t)n + 1;
        if (needed > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq_arr = tmp;
            seq_cap = new_cap;
        }
        /* compute terms from old_len to n inclusive */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            seq_t val;
            if (i == 0) val = 1;
            else if (i == 1) val = 2;
            else if ((i & 1) == 0) {          /* even index */
                val = (seq_t)3 * (seq_t)i - 2;
            } else {                          /* odd index, i >= 3 */
                size_t m = (i - 1) >> 1;      /* floor((i-1)/2) */
                seq_t cand1 = (seq_arr[2 * m] - 1) / 3;
                seq_t cand2 = seq_arr[2 * m] * 2;
                seq_t cand3 = seq_arr[2 * m - 1] * 2;
                seq_t best = 0;
                bool found = false;
                seq_t cand[3] = { cand1, cand2, cand3 };
                for (int j = 0; j < 3; ++j) {
                    seq_t v = cand[j];
                    if (is_forbidden(v)) continue;
                    if (!val_seen(v)) {
                        if (!found || v < best) {
                            best = v;
                            found = true;
                        }
                    }
                }
                /* The definition guarantees a candidate exists. */
                val = best;
            }
            seq_arr[i] = val;
            val_add(val);
        }
        seq_len = needed;
    }
    return seq_arr[(size_t)n];
}
