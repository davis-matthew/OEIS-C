/*
 * A337064  a(n) is the index of the first occurrence of n in A337063, or -1
 *          if n never appears.
 * https://oeis.org/A337064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- generate A337063 (a(0)=a(1)=1,
 *            a(k)=freq(a(k-1),k)*freq(a(k-2),k)) for its first 2*10^6 terms,
 *            recording the first index of each value; a(n) = that index, or
 *            -1 if n does not occur in that range
 * Range:     u8 n <= 18, u16 n <= 336, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9106 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A337064_LIMIT 2000000ULL

static uint64_t *A337064_keys = NULL;
static uint32_t *A337064_cnt = NULL;
static uint32_t *A337064_first = NULL;
static size_t A337064_bits = 0;
static size_t A337064_used = 0;

static size_t A337064_slot(uint64_t key)
{
    size_t mask = ((size_t)1 << A337064_bits) - 1;
    size_t h = (size_t)((key * 11400714819323198485ULL) >> (64 - A337064_bits));
    while (A337064_keys[h] != 0 && A337064_keys[h] != key)
        h = (h + 1) & mask;
    return h;
}

static void A337064_alloc(size_t bits)
{
    size_t sz = (size_t)1 << bits;
    A337064_bits = bits;
    A337064_keys = (uint64_t *)calloc(sz, sizeof(uint64_t));
    A337064_cnt = (uint32_t *)calloc(sz, sizeof(uint32_t));
    A337064_first = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!A337064_keys || !A337064_cnt || !A337064_first) abort();
}

static void A337064_grow(void)
{
    uint64_t *ok = A337064_keys;
    uint32_t *oc = A337064_cnt;
    uint32_t *of = A337064_first;
    size_t osz = (size_t)1 << A337064_bits;
    A337064_alloc(A337064_bits + 1);
    for (size_t i = 0; i < osz; i++) {
        if (ok[i] != 0) {
            size_t s = A337064_slot(ok[i]);
            A337064_keys[s] = ok[i];
            A337064_cnt[s] = oc[i];
            A337064_first[s] = of[i];
        }
    }
    free(ok);
    free(oc);
    free(of);
}

static void A337064_init(void)
{
    A337064_alloc(16);
    uint64_t p2 = 1, p1 = 1;
    size_t s = A337064_slot(1);
    A337064_keys[s] = 1;
    A337064_cnt[s] = 2;
    A337064_first[s] = 0;
    A337064_used = 1;
    for (uint64_t i = 2; i < A337064_LIMIT; i++) {
        uint64_t c1 = A337064_cnt[A337064_slot(p1)];
        uint64_t c2 = A337064_cnt[A337064_slot(p2)];
        uint64_t v = c1 * c2;
        size_t t = A337064_slot(v);
        if (A337064_keys[t] == 0) {
            A337064_keys[t] = v;
            A337064_first[t] = (uint32_t)i;
            A337064_cnt[t] = 1;
            A337064_used++;
            if (A337064_used * 2 > ((size_t)1 << A337064_bits))
                A337064_grow();
        } else {
            A337064_cnt[t]++;
        }
        p2 = p1;
        p1 = v;
    }
}

seq_t a(idx_t n)
{
    if (!A337064_keys) A337064_init();
    if (n != 0) {
        size_t s = A337064_slot((uint64_t)n);
        if (A337064_keys[s] == (uint64_t)n)
            return (seq_t)A337064_first[s];
    }
    seq_t r = 0;
    --r;
    return r;
}
