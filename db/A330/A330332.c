/*
 * A330332  a(n) = (number of times a(n-1) has already appeared) + (number of
 *          times a(n-2) has already appeared) + (number of times a(n-3) has
 *          already appeared), starting with a(n) = n for n<3.
 * https://oeis.org/A330332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (number of times a(n-1) has already appeared) + (number
 *            of times a(n-2) has already appeared) + (number of times a(n-3)
 *            has already appeared); Initial values: a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build. */

#include <stdlib.h>
#include <string.h>

/* ---------------- Hash table for frequencies ---------------- */

typedef struct {
    seq_t   key;
    size_t  cnt;
    uint8_t used;
} entry_t;

static entry_t *freq_tbl      = NULL;
static size_t   freq_cap      = 0;
static size_t   freq_used     = 0;

/* Simple hash function */
static size_t
hash_value(seq_t v)
{
    /* Cast to 64 bits for hashing; values are normally small */
    return (size_t)((unsigned long long)v * 11400714819323198485ULL);
}

static void
freq_init(size_t want)
{
    size_t cap = 1;
    while (cap < want) cap <<= 1;
    freq_tbl = (entry_t*)malloc(cap * sizeof(entry_t));
    memset(freq_tbl, 0, cap * sizeof(entry_t));
    freq_cap = cap;
    freq_used = 0;
}

static size_t
freq_get(seq_t key)
{
    size_t h = hash_value(key) & (freq_cap - 1);
    while (freq_tbl[h].used) {
        if (freq_tbl[h].key == key) return freq_tbl[h].cnt;
        h = (h + 1) & (freq_cap - 1);
    }
    return 0;
}

static void
freq_insert_new(seq_t key, size_t cnt)
{
    size_t h = hash_value(key) & (freq_cap - 1);
    while (freq_tbl[h].used) {
        if (freq_tbl[h].key == key) {
            freq_tbl[h].cnt = cnt;
            return;
        }
        h = (h + 1) & (freq_cap - 1);
    }
    freq_tbl[h].used = 1;
    freq_tbl[h].key  = key;
    freq_tbl[h].cnt  = cnt;
    freq_used++;
}

static void
freq_inc(seq_t key)
{
    size_t h = hash_value(key) & (freq_cap - 1);
    while (freq_tbl[h].used) {
        if (freq_tbl[h].key == key) {
            freq_tbl[h].cnt++;
            return;
        }
        h = (h + 1) & (freq_cap - 1);
    }
    freq_tbl[h].used = 1;
    freq_tbl[h].key  = key;
    freq_tbl[h].cnt  = 1;
    freq_used++;

    /* Resize if load factor > ~2/3 */
    if (freq_used * 3 > freq_cap * 2) {
        size_t old_cap = freq_cap;
        entry_t *old_tbl = freq_tbl;
        freq_cap <<= 1;
        freq_tbl = (entry_t*)malloc(freq_cap * sizeof(entry_t));
        memset(freq_tbl, 0, freq_cap * sizeof(entry_t));
        freq_used = 0;
        for (size_t i = 0; i < old_cap; ++i) {
            if (old_tbl[i].used) {
                freq_insert_new(old_tbl[i].key, old_tbl[i].cnt);
            }
        }
        free(old_tbl);
    }
}

/* ---------------- Sequence storage ---------------- */

static seq_t *seq_vals      = NULL;
static size_t seq_len       = 0;
static size_t seq_cap       = 0;

static void
seq_resize(size_t mincap)
{
    if (seq_cap >= mincap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 4;
    while (newcap < mincap) newcap *= 2;
    seq_vals = (seq_t*)realloc(seq_vals, newcap * sizeof(seq_t));
    seq_cap  = newcap;
}

/* ---------------- Main function ---------------- */

seq_t a(idx_t n)
{
    /* Lazy initialisation */
    if (!seq_vals) {
        seq_resize(16);
        freq_init(16);
        seq_len = 0;
    }

    /* Build the sequence up to n */
    while (seq_len <= (size_t)n) {
        seq_t value;
        if (seq_len < 3) {
            value = (seq_t)seq_len;          /* a(0)=0, a(1)=1, a(2)=2 */
        } else {
            seq_t v1 = seq_vals[seq_len - 1];
            seq_t v2 = seq_vals[seq_len - 2];
            seq_t v3 = seq_vals[seq_len - 3];
            size_t c1 = freq_get(v1);
            size_t c2 = freq_get(v2);
            size_t c3 = freq_get(v3);
            value = (seq_t)(c1 + c2 + c3);
        }
        seq_resize(seq_len + 1);
        seq_vals[seq_len] = value;
        freq_inc(value);
        seq_len++;
    }

    return seq_vals[(size_t)n];
}
