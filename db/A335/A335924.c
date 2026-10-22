/*
 * A335924  A variation on Recamán's sequence (A005132): a(0) = 0, a(n) =
 *          a(n-1) - n if nonnegative and not already in the sequence;
 *          otherwise, a(n) = a(n-1) + ceiling(n/2) if not already in the
 *          sequence and a(n) = a(n-1) + n if already in the sequence.
 * https://oeis.org/A335924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hash_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0:
   a(n)=a(n‑1)–n          if a(n‑1)≥n and (a(n‑1)–n) not seen before;
   else a(n)=a(n‑1)+ceil(n/2) if that value not seen before;
   else a(n)=a(n‑1)+n.  */
/* This implementation memoises the sequence and keeps a hash set of
   all values that have already appeared.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/hash_val.h"

/* ---------- simple hash set for seq_t values ---------- */

static seq_t *hash_keys   = NULL;   /* keys array          */
static unsigned char *hash_used = NULL; /* 0=empty, 1=used   */
static size_t hash_capacity = 0;  /* always power of two */
static size_t hash_count    = 0;  /* number of stored keys */

/* enlarge the hash table */
static void hash_expand(void)
{
    size_t new_cap = hash_capacity ? hash_capacity * 2 : 8;
    /* power‑of‑two growth */
    while (new_cap < hash_capacity * 2) new_cap <<= 1;

    seq_t *new_keys = (seq_t*)calloc(new_cap, sizeof(seq_t));
    unsigned char *new_used = (unsigned char*)calloc(new_cap, 1);
    if (!new_keys || !new_used) abort();

    /* re‑hash existing keys */
    if (hash_used) {
        for (size_t i = 0; i < hash_capacity; ++i) {
            if (hash_used[i]) {
                seq_t val = hash_keys[i];
                size_t idx = hash_val(val) & (new_cap - 1);
                while (new_used[idx]) idx = (idx + 1) & (new_cap - 1);
                new_keys[idx] = val;
                new_used[idx] = 1;
            }
        }
    }

    free(hash_keys);
    free(hash_used);
    hash_keys   = new_keys;
    hash_used   = new_used;
    hash_capacity = new_cap;
}

/* test membership */
static bool hash_contains(seq_t v)
{
    if (!hash_used) return false;
    size_t idx = hash_val(v) & (hash_capacity - 1);
    while (hash_used[idx]) {
        if (hash_keys[idx] == v) return true;
        idx = (idx + 1) & (hash_capacity - 1);
    }
    return false;
}

/* insert value (no duplicate) */
static void hash_insert(seq_t v)
{
    if (!hash_used) hash_expand();

    if (hash_contains(v)) return;          /* already present */

    /* keep load factor ≤0.7 */
    if ((double)(hash_count + 1) / hash_capacity > 0.7) hash_expand();

    size_t idx = hash_val(v) & (hash_capacity - 1);
    while (hash_used[idx]) idx = (idx + 1) & (hash_capacity - 1);
    hash_keys[idx]   = v;
    hash_used[idx]   = 1;
    ++hash_count;
}

/* ---------- cache for the sequence ---------- */

static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_cap = 0;      /* capacity of cache */
static idx_t  cache_len = 0;      /* number of valid terms (≤ cache_cap) */

/* ensure cache holds terms up to index n (inclusive) */
static void cache_extend(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    /* grow cache array if necessary */
    size_t needed = n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first term if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
        hash_insert((seq_t)0);
    }

    /* compute consecutive terms */
    for (idx_t i = cache_len; (idx_t)i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;

        /* try backward step */
        if (prev >= (seq_t)i) {
            seq_t sub = prev - (seq_t)i;
            if (!hash_contains(sub)) {
                next = sub;
                cache[i] = next;
                hash_insert(next);
                continue;
            }
        }

        /* try forward step of ceil(i/2) */
        seq_t half    = ((seq_t)i + 1) / 2;   /* ceil(i/2) */
        seq_t forward = prev + half;
        if (!hash_contains(forward)) {
            next = forward;
            cache[i] = next;
            hash_insert(next);
            continue;
        }

        /* fallback: add i */
        next = prev + (seq_t)i;
        cache[i] = next;
        hash_insert(next);
    }

    cache_len = n + 1;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    cache_extend(n);
    return cache[n];
}
