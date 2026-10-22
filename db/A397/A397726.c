/*
 * A397726  a(0) = 0; for k = 0, 1, ..., append a(k) + r^2 and a(k) + s^2 with
 *          0 < r < s as small as possible so as to avoid duplicates.
 * https://oeis.org/A397726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   computes sequence A397726 * a(0) = 0; for k = 0, 1, ... append
 *            a(k)+r^2 and a(k)+s^2 * with 0 < r < s as small as possible so
 *            as to avoid duplicates. * Implements a breadth‑first
 *            construction with memoisation
 * Range:     u8 n <= 207, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Hash set for values that already appeared in the sequence --- */
typedef struct {
    seq_t *table;          /* values */
    bool *used;            /* occupancy flag */
    size_t size;           /* size of table (power of two) */
    size_t used_count;     /* number of used slots */
} visited_hash_t;

static visited_hash_t VH = {0};

static size_t hash_seq_t(seq_t x)
{
    /* Mix high bits if seq_t wider than size_t */
    if (sizeof(seq_t) <= sizeof(size_t))
        return (size_t)x;
    return (size_t)(x ^ (x >> 64));
}

static void visited_resize(size_t new_size)
{
    visited_hash_t *h = &VH;
    visited_hash_t new_vh;
    new_vh.size = new_size;
    new_vh.used_count = 0;
    new_vh.table = (seq_t*)calloc(new_size, sizeof(seq_t));
    new_vh.used = (bool*)calloc(new_size, sizeof(bool));
    if (!new_vh.table || !new_vh.used)
        exit(1); /* out of memory */

    for (size_t i = 0; i < h->size; ++i)
        if (h->used[i]) {
            seq_t v = h->table[i];
            size_t idx = hash_seq_t(v) & (new_vh.size - 1);
            while (new_vh.used[idx])
                idx = (idx + 1) & (new_vh.size - 1);
            new_vh.table[idx] = v;
            new_vh.used[idx] = true;
            new_vh.used_count++;
        }
    free(h->table);
    free(h->used);
    *h = new_vh;
}

static void visited_init(void)
{
    if (VH.table) return;
    visited_resize(64);          /* initial size */
    /* The sequence starts with 0 */
    seq_t zero = 0;
    size_t idx = hash_seq_t(zero) & (VH.size - 1);
    VH.table[idx] = zero;
    VH.used[idx] = true;
    VH.used_count = 1;
}

static bool visited_contains(seq_t v)
{
    visited_init();
    size_t idx = hash_seq_t(v) & (VH.size - 1);
    while (VH.used[idx]) {
        if (VH.table[idx] == v)
            return true;
        idx = (idx + 1) & (VH.size - 1);
    }
    return false;
}

static void visited_insert(seq_t v)
{
    if (!visited_contains(v)) {
        if ((VH.used_count + 1) * 2 > VH.size)   /* load factor > 0.5 */
            visited_resize(VH.size * 2);
        size_t idx = hash_seq_t(v) & (VH.size - 1);
        while (VH.used[idx])
            idx = (idx + 1) & (VH.size - 1);
        VH.table[idx] = v;
        VH.used[idx] = true;
        VH.used_count++;
    }
}

/* --- Sequence storage --- */
static seq_t *SEQ = NULL;      /* array of terms */
static size_t SEQ_CAP = 0;     /* capacity of SEQ */
static size_t SEQ_LEN = 0;     /* number of terms stored */

static void seq_ensure_cap(size_t need)
{
    if (need <= SEQ_CAP)
        return;
    size_t new_cap = SEQ_CAP ? SEQ_CAP * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(SEQ, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    SEQ = tmp;
    SEQ_CAP = new_cap;
}

/* --- Main function --- */
seq_t a(idx_t n)
{
    static idx_t next_k = 0;       /* next index to process */

    /* initialise sequence array with the first term */
    if (SEQ_LEN == 0) {
        seq_ensure_cap(1);
        SEQ[0] = 0;
        SEQ_LEN = 1;
    }

    /* make sure values are known to be visited */
    visited_init();

    /* generate terms until we have computed index n */
    while (SEQ_LEN <= (size_t)n) {
        seq_t base = SEQ[next_k];

        /* find smallest r > 0 with base + r^2 not used */
        idx_t r = 1;
        seq_t val_r;
        for (;;) {
            seq_t sq = (seq_t)r * (seq_t)r;
            val_r = base + sq;
            if (!visited_contains(val_r))
                break;
            ++r;
        }

        /* find smallest s > r with base + s^2 not used */
        idx_t s = r + 1;
        seq_t val_s;
        for (;;) {
            seq_t sq = (seq_t)s * (seq_t)s;
            val_s = base + sq;
            if (!visited_contains(val_s))
                break;
            ++s;
        }

        /* append two new terms */
        seq_ensure_cap(SEQ_LEN + 2);
        SEQ[SEQ_LEN]     = val_r;
        SEQ[SEQ_LEN + 1] = val_s;
        visited_insert(val_r);
        visited_insert(val_s);

        SEQ_LEN += 2;
        ++next_k;
    }

    return SEQ[(size_t)n];
}
