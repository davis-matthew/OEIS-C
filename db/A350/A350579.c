/*
 * A350579  The first number in A350578 to appear n times.
 * https://oeis.org/A350579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulate A350578 (a(0)=0; a(k)=a(k-1)-k if >=0 and
 *            count(a(k-1)-k) <= count(a(k-1)+k), else a(k-1)+k); a(n) = the
 *            value whose occurrence count first reaches n during the
 *            simulation
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

struct A350579_Entry {
    bool used;
    seq_t key;
    unsigned cnt;
};

static struct A350579_Entry *A350579_tbl = NULL;
static size_t A350579_cap = 0;
static size_t A350579_n = 0;

static seq_t *A350579_res = NULL;
static unsigned A350579_maxc = 0;
static seq_t A350579_prev = 0;
static idx_t A350579_step = 0;
#define A350579_RES 4096

static size_t A350579_hash(seq_t x)
{
    uint64_t h = (uint64_t)(unsigned long long)(x % (seq_t)1000003);
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return (size_t)h;
}

static struct A350579_Entry *A350579_find(seq_t key)
{
    size_t m = A350579_cap - 1;
    size_t j = A350579_hash(key) & m;
    while (A350579_tbl[j].used && !(A350579_tbl[j].key == key))
        j = (j + 1) & m;
    return &A350579_tbl[j];
}

static unsigned A350579_get(seq_t key)
{
    struct A350579_Entry *e = A350579_find(key);
    return e->used ? e->cnt : 0;
}

static void A350579_grow(void)
{
    size_t oc = A350579_cap;
    struct A350579_Entry *old = A350579_tbl;
    A350579_cap = oc ? oc * 2 : 1024;
    A350579_tbl = mem_new(struct A350579_Entry, A350579_cap);
    for (size_t i = 0; i < oc; ++i) {
        if (old[i].used) {
            struct A350579_Entry *e = A350579_find(old[i].key);
            e->used = true;
            e->key = old[i].key;
            e->cnt = old[i].cnt;
        }
    }
    if (old) mem_free(old);
}

static unsigned A350579_inc(seq_t key)
{
    if ((A350579_n + 1) * 2 > A350579_cap) A350579_grow();
    struct A350579_Entry *e = A350579_find(key);
    if (!e->used) {
        e->used = true;
        e->key = key;
        e->cnt = 0;
        ++A350579_n;
    }
    return ++e->cnt;
}

seq_t a(idx_t n)
{
    if (!A350579_res) {
        A350579_res = mem_new(seq_t, A350579_RES);
        A350579_inc((seq_t)0);
        A350579_res[1] = (seq_t)0;
        A350579_maxc = 1;
        A350579_prev = (seq_t)0;
        A350579_step = 0;
    }
    if (n >= A350579_RES) return (seq_t)0;
    while (A350579_maxc < n) {
        A350579_step++;
        seq_t k = (seq_t)A350579_step;
        seq_t p = A350579_prev;
        seq_t plus = p + k;
        seq_t nxt;
        if (p >= k) {
            seq_t minus = p - k;
            nxt = (A350579_get(minus) <= A350579_get(plus)) ? minus : plus;
        } else {
            nxt = plus;
        }
        unsigned c = A350579_inc(nxt);
        if (c > A350579_maxc) {
            A350579_maxc = c;
            A350579_res[c] = nxt;
        }
        A350579_prev = nxt;
    }
    return A350579_res[n];
}
