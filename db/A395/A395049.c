/*
 * A395049  Indices of odd terms of A084451.
 * https://oeis.org/A395049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k (counting from k=0) of the n-th odd term of
 *            A084451, where A084451(0)=7, A084451(k)=A084451(k-1)/2 if that
 *            is even, else (A084451(k-1)-1)/2+2^e with e the least value such
 *            that the result is new and has >=3 one-bits; computed by direct
 *            simulation
 * Range:     u8 n <= 37, u16 n <= 288, u32 n <= 2882, u64 n <= 22697,
 *            u128 n <= 22697, big any n
 * Validated: standard, pass, 90 terms through n = 9886 (2026-11-10)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t *A395049_terms = NULL;
static size_t A395049_len = 0, A395049_cap = 0;
static idx_t *A395049_odd = NULL;
static size_t A395049_ocnt = 0, A395049_ocap = 0;
static uint32_t *A395049_tab = NULL;
static size_t A395049_tcap = 0;

static size_t A395049_hash(seq_t x)
{
    uint64_t lo = (uint64_t)(unsigned long long)(x & (seq_t)0xFFFFFFFFu);
    seq_t y = (x >> 16) >> 16;
    uint64_t mid = (uint64_t)(unsigned long long)(y & (seq_t)0xFFFFFFFFu);
    uint64_t h = lo * 0x9E3779B97F4A7C15ULL ^ (mid + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return (size_t)h;
}

static bool A395049_find(seq_t x)
{
    size_t m = A395049_tcap - 1;
    size_t p = A395049_hash(x) & m;
    while (A395049_tab[p])
    {
        if (A395049_terms[A395049_tab[p] - 1] == x) return true;
        p = (p + 1) & m;
    }
    return false;
}

static void A395049_tab_insert(size_t idx)
{
    size_t m = A395049_tcap - 1;
    size_t p = A395049_hash(A395049_terms[idx]) & m;
    while (A395049_tab[p]) p = (p + 1) & m;
    A395049_tab[p] = (uint32_t)(idx + 1);
}

static void A395049_rehash(size_t newcap)
{
    free(A395049_tab);
    A395049_tab = (uint32_t *)calloc(newcap, sizeof(uint32_t));
    if (!A395049_tab) exit(EXIT_FAILURE);
    A395049_tcap = newcap;
    for (size_t i = 0; i < A395049_len; ++i) A395049_tab_insert(i);
}

static int A395049_pop(seq_t x)
{
    int c = 0;
    while (x != 0)
    {
        x &= x - (seq_t)1;
        c++;
    }
    return c;
}

static void A395049_push(seq_t v)
{
    if (A395049_len + 1 > A395049_cap)
    {
        size_t nc = A395049_cap ? A395049_cap * 2 : 1024;
        seq_t *t = mem_resize(seq_t, A395049_terms, nc);
        if (!t) exit(EXIT_FAILURE);
        A395049_terms = t;
        A395049_cap = nc;
    }
    if ((A395049_len + 1) * 2 > A395049_tcap)
    {
        A395049_terms[A395049_len] = v;
        size_t nt = A395049_tcap ? A395049_tcap * 2 : 4096;
        /* rehash existing entries first (len not yet incremented) */
        free(A395049_tab);
        A395049_tab = (uint32_t *)calloc(nt, sizeof(uint32_t));
        if (!A395049_tab) exit(EXIT_FAILURE);
        A395049_tcap = nt;
        for (size_t i = 0; i < A395049_len; ++i) A395049_tab_insert(i);
    }
    A395049_terms[A395049_len] = v;
    A395049_tab_insert(A395049_len);
    if ((v & (seq_t)1) != 0)
    {
        if (A395049_ocnt + 1 > A395049_ocap)
        {
            size_t nc = A395049_ocap ? A395049_ocap * 2 : 1024;
            idx_t *t = (idx_t *)realloc(A395049_odd, nc * sizeof(idx_t));
            if (!t) exit(EXIT_FAILURE);
            A395049_odd = t;
            A395049_ocap = nc;
        }
        A395049_odd[A395049_ocnt++] = (idx_t)A395049_len;
    }
    A395049_len++;
}

seq_t a(idx_t n)
{
    if (A395049_len == 0) A395049_push((seq_t)7);
    while (A395049_ocnt < (size_t)n)
    {
        seq_t prev = A395049_terms[A395049_len - 1];
        seq_t next;
        if ((prev & (seq_t)1) == 0)
        {
            next = prev >> 1;
        }
        else
        {
            seq_t base = (prev - (seq_t)1) >> 1;
            idx_t e = 0;
            next = base;
            while (e < 130)
            {
                seq_t cand = base + pow2(e);
                if (!A395049_find(cand) && A395049_pop(cand) >= 3)
                {
                    next = cand;
                    break;
                }
                ++e;
            }
        }
        A395049_push(next);
    }
    return (seq_t)A395049_odd[(size_t)n - 1];
}
