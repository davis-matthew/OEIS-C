/*
 * A334944  For any n > 0, let w be the least positive number such that the
 *          values (a(n+1-w), ..., a(n-1), e) do not appear continuously in
 *          (a(1), ..., a(n-1)) for some e in 0..w-1; a(n) is the least such
 *          e.
 * https://oeis.org/A334944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A334944 by exhaustive search using a rolling hash.
   The algorithm keeps, for each possible pattern length w, the set of all
   contiguous subsequences of that length that have appeared in the
   already generated part of the sequence.  For each new term n the
   smallest e (0 ≤ e < w) for which the pattern
   (a(n+1‑w), … , a(n‑1), e) has not yet occurred is found.
   The implementation uses an unsigned 64‑bit rolling hash; this
   probability of collision is negligible for the size ranges used by
   the tests.  Memoisation allows calls in arbitrary order. */

/* include only standard headers required for the implementation */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* internal hash set for 64‑bit keys (key+1 stored to allow 0 as empty) */
typedef struct {
    size_t capacity;   /* power of two */
    size_t used;       /* number of elements */
    uint64_t *bucket;  /* key+1, 0 means empty */
} hset_t;

static void hset_init(hset_t *h)
{
    h->capacity = 16;
    h->used = 0;
    h->bucket = (uint64_t *)calloc(h->capacity, sizeof(uint64_t));
}

static void hset_resize(hset_t *h, size_t newcap)
{
    uint64_t *newb = (uint64_t *)calloc(newcap, sizeof(uint64_t));
    size_t mask = newcap - 1;
    for (size_t i = 0; i < h->capacity; ++i) {
        uint64_t v = h->bucket[i];
        if (v == 0) continue;
        size_t j = v & mask;            /* because v = key+1 */
        while (newb[j]) j = (j + 1) & mask;
        newb[j] = v;
    }
    free(h->bucket);
    h->bucket = newb;
    h->capacity = newcap;
}

static bool hset_find(const hset_t *h, uint64_t key)
{
    size_t idx = (key + 1) & (h->capacity - 1);
    while (true) {
        uint64_t v = h->bucket[idx];
        if (v == 0) return false;
        if (v == key + 1) return true;
        idx = (idx + 1) & (h->capacity - 1);
    }
}

static void hset_insert(hset_t *h, uint64_t key)
{
    if ((h->used * 2) >= h->capacity) {
        hset_resize(h, h->capacity * 2);
    }
    size_t idx = (key + 1) & (h->capacity - 1);
    while (true) {
        uint64_t v = h->bucket[idx];
        if (v == 0) {
            h->bucket[idx] = key + 1;
            h->used++;
            return;
        }
        if (v == key + 1) return;  /* already present */
        idx = (idx + 1) & (h->capacity - 1);
    }
}

/* --------- global state --------- */
static seq_t *seq = NULL;           /* generated sequence */
static size_t seq_cap = 0;
static size_t seq_len = 0;

static uint64_t *pre_hash = NULL;   /* prefix hashes */
static size_t pre_cap = 0;

static uint64_t *pow_base = NULL;   /* powers of BASE */
static size_t pow_cap = 0;

static hset_t *seen = NULL;         /* array of hash sets per w */
static size_t seen_cnt = 0;         /* number of hash sets allocated */

/* rolling‑hash base (prime) */
static const uint64_t BASE = 911382323ULL;

/* --- utility: enlarge storage if needed --- */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t newc = seq_cap ? seq_cap * 2 : 16;
    while (newc < need) newc <<= 1;
    seq = (seq_t *)realloc(seq, newc * sizeof(seq_t));
    pre_hash = (uint64_t *)realloc(pre_hash, newc * sizeof(uint64_t));
    seq_cap = newc;
}

static void ensure_pow_cap(size_t need)
{
    if (need <= pow_cap) return;
    size_t newc = pow_cap ? pow_cap * 2 : 16;
    while (newc < need) newc <<= 1;
    pow_base = (uint64_t *)realloc(pow_base, newc * sizeof(uint64_t));
    if (pow_cap == 0) {
        pow_base[0] = 1ULL;
        pow_cap = 1;
    }
    for (size_t i = pow_cap; i < newc; ++i)
        pow_base[i] = pow_base[i-1] * BASE;
    pow_cap = newc;
}

static void ensure_seen_cap(size_t need)
{
    if (need <= seen_cnt) return;
    size_t newc = seen_cnt ? seen_cnt * 2 : 16;
    while (newc < need) newc <<= 1;
    seen = (hset_t *)realloc(seen, newc * sizeof(hset_t));
    for (size_t i = seen_cnt; i < newc; ++i) hset_init(&seen[i]);
    seen_cnt = newc;
}

/* hash of subsequence [l .. r] inclusive, 0‑based */
static inline uint64_t sub_hash(size_t l, size_t r)
{
    if (l == 0) return pre_hash[r];
    return pre_hash[r] - pre_hash[l-1] * pow_base[r - l + 1];
}

/* insert all new substrings of length w that end at the newest element
   (index newest) into seen[w] */
static void update_seen_sets(size_t newest)
{
    for (size_t w = 1; w < seen_cnt; ++w) {
        if (newest + 1 < w) continue;              /* substring too short */
        size_t l = newest + 1 - w;
        size_t r = newest;
        uint64_t h = sub_hash(l, r);
        hset_insert(&seen[w], h);
    }
}

/* ------- main sequence generator -------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safety */

    size_t target = (size_t)n;

    /* initialise once */
    if (seq_len == 0) {
        ensure_seq_cap(16);
        ensure_pow_cap(16);
        ensure_seen_cap(64);
    }

    /* generate terms up to target */
    while (seq_len < target) {
        size_t pos = seq_len;                       /* index of new term, 0‑based */

        /* find minimal e for some w */
        seq_t val = 0;
        for (size_t w = 1; w <= pos + 1; ++w) {
            ensure_seen_cap(w + 1);                 /* we need seen[w] */
            uint64_t hash_old;
            if (w == 1) {
                hash_old = 0;                       /* empty product */
            } else {
                size_t start = pos + 1 - w;          /* first index of the w‑1 suffix */
                size_t end   = pos - 1;              /* last index of the w‑1 suffix */
                hash_old = sub_hash(start, end);
            }

            bool found = false;
            for (uint64_t e = 0; e < w; ++e) {
                uint64_t h = hash_old * BASE + e;
                if (!hset_find(&seen[w], h)) {
                    val = (seq_t)e;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        /* append new term */
        seq_len++;
        ensure_seq_cap(seq_len);
        seq[seq_len-1] = val;

        /* compute prefix hash for this term */
        if (seq_len == 1) {
            pre_hash[0] = (uint64_t)val;
        } else {
            pre_hash[seq_len-1] = pre_hash[seq_len-2] * BASE + (uint64_t)val;
        }

        ensure_pow_cap(seq_len);

        /* update the sets of seen substrings with the new element */
        update_seen_sets(seq_len - 1);
    }

    return seq[target-1];
}
