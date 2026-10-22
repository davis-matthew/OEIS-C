/*
 * A386551  Numbers that can terminate in 1 by successively dividing a
 *          contiguous substring of digits by 2.
 * https://oeis.org/A386551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   reverse BFS starting from 1; generate increasing numbers that
 *            can reach 1 by repeatedly dividing a non‑zero, non‑leading‑zero
 *            even digit substring by 2. The sequence is filled lazily, so
 *            calls may be in any order
 * Range:     u8 n <= 13, u16 n <= 3250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_dec_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_dec_str.h"

/* -------------------------------------------------------------------
   Utility: convert seq_t to decimal string and back
   ------------------------------------------------------------------- */
static void seq_to_dec_str(seq_t val, char *buf, size_t *len)
{
    if (val == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    size_t pos = 0;
    seq_t tmp = val;
    while (tmp != (seq_t)0) {
        seq_t digit = tmp % (seq_t)10;
        buf[pos++] = (char)('0' + (int)digit);
        tmp /= (seq_t)10;
    }
    /* reverse */
    for (size_t i = 0; i < pos / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = c;
    }
    *len = pos;
    buf[pos] = '\0';
}

/* -------------------------------------------------------------------
   Visited set – open addressing hash table (0 reserved)
   ------------------------------------------------------------------- */
typedef struct {
    size_t size;   /* number of used slots (excluding 0) */
    size_t cap;    /* capacity (power of two) */
    seq_t *keys;   /* 0 means empty */
} VisSet;

static void vis_init(VisSet *vs)
{
    vs->size = 0;
    vs->cap  = 64;                    /* start with 64 slots */
    vs->keys = (seq_t *)calloc(vs->cap, sizeof(seq_t));
}

static void vis_resize(VisSet *vs)
{
    size_t newcap = vs->cap * 2;
    seq_t *newkeys = (seq_t *)calloc(newcap, sizeof(seq_t));
    for (size_t i = 0; i < vs->cap; ++i) {
        seq_t k = vs->keys[i];
        if (k == (seq_t)0) continue;
        size_t h = (size_t)k ^ ((size_t)k >> 32);
        h &= newcap - 1;
        while (newkeys[h] != (seq_t)0) {
            h = (h + 1) & (newcap - 1);
        }
        newkeys[h] = k;
    }
    free(vs->keys);
    vs->keys = newkeys;
    vs->cap  = newcap;
}

static bool vis_contains(const VisSet *vs, seq_t key)
{
    if (key == (seq_t)0) return false;
    size_t h = (size_t)key ^ ((size_t)key >> 32);
    h &= vs->cap - 1;
    while (true) {
        seq_t k = vs->keys[h];
        if (k == (seq_t)0) return false;
        if (k == key) return true;
        h = (h + 1) & (vs->cap - 1);
    }
}

static void vis_add(VisSet *vs, seq_t key)
{
    if (key == (seq_t)0) return;
    if ((vs->size + 1) * 2 > vs->cap) vis_resize(vs); /* load factor > 0.5 */
    size_t h = (size_t)key ^ ((size_t)key >> 32);
    h &= vs->cap - 1;
    while (vs->keys[h] != (seq_t)0) {
        h = (h + 1) & (vs->cap - 1);
    }
    vs->keys[h] = key;
    vs->size++;
}

/* -------------------------------------------------------------------
   Min‑heap of seq_t
   ------------------------------------------------------------------- */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} MinHeap;

static void h_init(MinHeap *h)
{
    h->data = NULL;
    h->size = 0;
    h->cap  = 0;
}

static void h_reserve(MinHeap *h, size_t need)
{
    if (h->cap < need) {
        size_t newcap = h->cap ? h->cap * 2 : 64;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(h->data, newcap * sizeof(seq_t));
        if (!tmp) abort();
        h->data = tmp;
        h->cap  = newcap;
    }
}

static void h_push(MinHeap *h, seq_t v)
{
    h_reserve(h, h->size + 1);
    size_t i = h->size++;
    h->data[i] = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p] <= h->data[i]) break;
        seq_t t = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = t;
        i = p;
    }
}

static seq_t h_pop(MinHeap *h)
{
    seq_t top = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < h->size && h->data[r] < h->data[l]) smallest = r;
        if (h->data[smallest] >= h->data[i]) break;
        seq_t t = h->data[smallest];
        h->data[smallest] = h->data[i];
        h->data[i] = t;
        i = smallest;
    }
    return top;
}

/* -------------------------------------------------------------------
   Dynamic array for sequence storage
   ------------------------------------------------------------------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void seq_append(seq_t val)
{
    if (seq_cap == 0) {
        seq_cap = 64;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();
    } else if (seq_len == seq_cap) {
        seq_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
    }
    seq_arr[seq_len++] = val;
}

/* -------------------------------------------------------------------
   Main function
   ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence offset is 1 */

    /* Static initialized once */
    static bool initialized = false;
    static MinHeap heap;
    static VisSet vis;

    if (!initialized) {
        h_init(&heap);
        vis_init(&vis);
        seq_t one = (seq_t)1;
        vis_add(&vis, one);
        h_push(&heap, one);
        initialized = true;
    }

    while (seq_len < (size_t)n) {
        seq_t cur = h_pop(&heap);

        /* record in sequence */
        seq_append(cur);

        /* convert current number to string */
        char curbuf[256], newbuf[512];
        size_t curlen;
        seq_to_dec_str(cur, curbuf, &curlen);
        curbuf[curlen] = '\0';

        /* generate all reverse moves */
        for (size_t i = 0; i < curlen; ++i) {
            if (curbuf[i] == '0') continue; /* cannot start with 0 */
            seq_t sub = (seq_t)0;
            for (size_t j = i; j < curlen; ++j) {
                /* accumulate substring value */
                sub = sub * (seq_t)10 + (seq_t)(curbuf[j] - '0');

                /* multiply by 2 to get predecessor */
                seq_t dbl = sub * (seq_t)2;
                char dblbuf[128];
                size_t dbl_len;
                seq_to_dec_str(dbl, dblbuf, &dbl_len);

                /* build new string: prefix + dbl + suffix */
                size_t pos = 0;
                /* prefix */
                for (size_t k = 0; k < i; ++k) newbuf[pos++] = curbuf[k];
                /* doubled substring */
                for (size_t k = 0; k < dbl_len; ++k) newbuf[pos++] = dblbuf[k];
                /* suffix */
                for (size_t k = j + 1; k < curlen; ++k) newbuf[pos++] = curbuf[k];
                newbuf[pos] = '\0';

                seq_t next = seq_from_dec_str(newbuf, pos);

                if (!vis_contains(&vis, next)) {
                    vis_add(&vis, next);
                    h_push(&heap, next);
                }
            }
        }
    }

    return seq_arr[n - 1];
}
