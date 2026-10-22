/*
 * A349944  a (1) = 1; a(n) is the smallest number not yet present in the
 *          sequence such that the concatenation of a(n-1) and a(n) contains
 *          three consecutive descending digits d > e > f.
 * https://oeis.org/A349944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   has_desc_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349944
   a(1) = 1
   For n>1, a(n) is the smallest unused positive integer such that the
   decimal concatenation of a(n‑1) and a(n) contains three consecutive
   strictly decreasing digits d > e > f.
   The implementation builds the sequence iteratively and keeps track of
   used integers in a simple open‑addressing hash set.  It is fully
   portable to C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_desc_triple.h"

/* ---------- helper: digit handling ----------------------------------- */
static void get_digits(seq_t v, uint8_t *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    uint8_t rev[64];
    size_t revlen = 0;
    while (v != 0 && revlen < sizeof(rev)) {
        rev[revlen++] = (uint8_t)(v % (seq_t)10);
        v /= (seq_t)10;
    }
    for (size_t i = 0; i < revlen; ++i)
        buf[i] = rev[revlen - 1 - i];
    *len = revlen;
}

static bool has_desc_triple_of(seq_t v)
{
    uint8_t digits[64];
    size_t len;
    get_digits(v, digits, &len);
    return has_desc_triple(digits, len);
}

static bool has_desc_triple_concat(seq_t a, seq_t b)
{
    uint8_t da[64], db[64], combo[128];
    size_t la, lb;
    get_digits(a, da, &la);
    get_digits(b, db, &lb);
    size_t i = 0;
    for (size_t j = 0; j < la; ++j) combo[i++] = da[j];
    for (size_t j = 0; j < lb; ++j) combo[i++] = db[j];
    return has_desc_triple(combo, i);
}

/* ---------- simple open-address hash set for used integers --------- */
typedef struct {
    idx_t *data;   /* array of keys, 0 means empty (no sequence value is 0) */
    size_t cap;    /* size of data array (power of two) */
    size_t used;   /* number of occupied slots */
} HashSet;

static size_t hash_key(idx_t key, size_t cap)
{
    return ((key ^ (key >> 32)) & (cap - 1));
}

static void hash_set_init(HashSet *hs)
{
    hs->cap  = 16;
    hs->used = 0;
    hs->data = (idx_t *)calloc(hs->cap, sizeof(idx_t));
}

static void hash_set_free(HashSet *hs)
{
    free(hs->data);
    hs->data = NULL;
    hs->cap  = 0;
    hs->used = 0;
}

static bool hash_set_contains(const HashSet *hs, idx_t key)
{
    if (hs->used == 0) return false;
    size_t idx = hash_key(key, hs->cap);
    while (hs->data[idx] != 0) {
        if (hs->data[idx] == key) return true;
        idx = (idx + 1) & (hs->cap - 1);
    }
    return false;
}

static void hash_set_resize(HashSet *hs)
{
    size_t new_cap = hs->cap << 1;
    idx_t *new_data = (idx_t *)calloc(new_cap, sizeof(idx_t));
    for (size_t i = 0; i < hs->cap; ++i) {
        idx_t val = hs->data[i];
        if (val != 0) {
            size_t idx = hash_key(val, new_cap);
            while (new_data[idx] != 0) idx = (idx + 1) & (new_cap - 1);
            new_data[idx] = val;
        }
    }
    free(hs->data);
    hs->data = new_data;
    hs->cap  = new_cap;
}

static void hash_set_insert(HashSet *hs, idx_t key)
{
    /* grow if load factor exceeds 3/4 */
    if (hs->used * 4 >= hs->cap * 3) hash_set_resize(hs);
    size_t idx = hash_key(key, hs->cap);
    while (hs->data[idx] != 0) {
        if (hs->data[idx] == key) return;
        idx = (idx + 1) & (hs->cap - 1);
    }
    hs->data[idx] = key;
    hs->used++;
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    HashSet used;
    hash_set_init(&used);

    idx_t next_unused = 1;          /* smallest unused integer */
    seq_t prev = (seq_t)1;          /* a(1) */
    hash_set_insert(&used, (idx_t)1);
    next_unused = 2;                /* advance past 1 */

    for (idx_t pos = 1; pos < n; ++pos) {
        bool prev_has = has_desc_triple_of(prev);
        seq_t next_val;

        if (prev_has) {            /* any unused integer works */
            while (hash_set_contains(&used, next_unused)) ++next_unused;
            next_val = (seq_t)next_unused;
            hash_set_insert(&used, next_unused);
            ++next_unused;
            while (hash_set_contains(&used, next_unused)) ++next_unused;
        } else {                   /* need to find smallest unused that
                                      creates a decreasing triple */
            idx_t cand = 1;
            for (;;) {
                if (!hash_set_contains(&used, cand)) {
                    if (has_desc_triple_concat(prev, cand)) {
                        next_val = (seq_t)cand;
                        hash_set_insert(&used, cand);
                        if (cand == next_unused) {
                            ++next_unused;
                            while (hash_set_contains(&used, next_unused))
                                ++next_unused;
                        }
                        break;
                    }
                }
                ++cand;
            }
        }
        prev = next_val;
    }

    hash_set_free(&used);
    return prev;
}
