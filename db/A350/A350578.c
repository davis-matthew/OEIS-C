/*
 * A350578  a(0) = 0; for n > 0, if a(n-1) - n >= 0 and a(n-1) - n has
 *          appeared the same or fewer times than a(n-1) + n, then a(n) =
 *          a(n-1) - n. Otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A350578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350578:
   a(0) = 0;
   for n > 0:
      let x = a(n-1).
      If x >= n and count(x-n) <= count(x+n) then set a(n) = x-n,
      otherwise set a(n) = x+n.
   Here count(v) is the number of times the value v has appeared
   in the sequence up to index n-1 (i.e. not including a(n) itself). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* --- small 64‑bit mixing function --------------------------------- */
static inline uint64_t mix64(uint64_t h)
{
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return h;
}

/* ---------- Memoised sequence ------------------------------------- */
static seq_t *seq = NULL;       /* sequence values */
static idx_t seq_len = 0;       /* number of computed terms */
static size_t seq_cap = 0;      /* allocated capacity of seq */

/* ---------- Hash table for counts --------------------------------- */
struct HashEntry {
    bool used;
    seq_t key;
    int count;
};

static struct HashEntry *hash_tbl = NULL;
static size_t hash_cap = 0;   /* power of two */
static size_t hash_used = 0;

/* --- turn a seq_t into a 64‑bit hash value ------------------------ */
static uint64_t hash_seq_t(seq_t x)
{
    /* Use the decimal string representation as hash source */
    char buf[4096];
    size_t len = sizeof(buf);
    seq_to_str(x, buf, &len);          /* len is number of bytes written */
    uint64_t h = 0;
    for (size_t i = 0; i < len; ++i)
        h = h * 131 + (unsigned char)buf[i];
    return h;
}

/* --- initialise the hash table ------------------------------------ */
static void hash_init(size_t cap)
{
    hash_cap = cap;
    hash_tbl = mem_new(struct HashEntry, cap);
}

/* --- hash index (mask because cap is power of two) ---------------- */
static inline size_t hash_idx(seq_t key)
{
    return mix64(hash_seq_t(key)) & (hash_cap - 1);
}

/* --- look up count ------------------------------------------------- */
static int hash_get(seq_t key)
{
    if (!hash_tbl) return 0;
    size_t idx = hash_idx(key);
    for (size_t i = 0; i < hash_cap; ++i) {
        size_t j = (idx + i) & (hash_cap - 1);
        if (!hash_tbl[j].used) return 0;
        if (hash_tbl[j].key == key) return hash_tbl[j].count;
    }
    return 0;
}

/* --- increment count (insert if missing) --------------------------- */
static void hash_increment(seq_t key)
{
    if (!hash_tbl) hash_init(65536);          /* sufficient for n ≤ 10000 */
    size_t idx = hash_idx(key);
    for (size_t i = 0; i < hash_cap; ++i) {
        size_t j = (idx + i) & (hash_cap - 1);
        if (!hash_tbl[j].used) {
            hash_tbl[j].used = true;
            hash_tbl[j].key = key;
            hash_tbl[j].count = 1;
            ++hash_used;
            return;
        }
        if (hash_tbl[j].key == key) {
            ++hash_tbl[j].count;
            return;
        }
    }
}

/* --- wrappers ------------------------------------------------------ */
static inline int get_count(seq_t v)  { return hash_get(v); }
static inline void inc_count(seq_t v) { hash_increment(v); }

/* ---------- Sequence growth --------------------------------------- */
static void seq_grow(idx_t need)
{
    if (seq_cap >= need) return;
    size_t cap = seq_cap ? seq_cap : 16;
    while (cap < need) cap <<= 1;
    seq = mem_resize(seq_t, seq, cap);
    seq_cap = cap;
}

/* ---------- Main function ----------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure the array is large enough */
    if (n >= seq_len) {
        seq_grow(n + 1);
        if (seq_len == 0) {
            seq[0] = (seq_t)0;
            inc_count((seq_t)0);
            seq_len = 1;
        }
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_t prev = seq[i - 1];
            bool minus_ok = (prev >= (seq_t)i);
            seq_t cand_minus = 0, cand_plus = 0;
            if (minus_ok) cand_minus = prev - (seq_t)i;
            cand_plus = prev + (seq_t)i;
            if (minus_ok) {
                int cnt_minus = get_count(cand_minus);
                int cnt_plus  = get_count(cand_plus);
                seq[i] = (cnt_minus <= cnt_plus) ? cand_minus : cand_plus;
            } else {
                seq[i] = cand_plus;
            }
            inc_count(seq[i]);
        }
        seq_len = n + 1;
    }
    return seq[n];
}
