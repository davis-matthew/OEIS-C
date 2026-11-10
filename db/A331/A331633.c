/*
 * A331633  Distinct values of A055944 in order of their appearance as n
 *          grows.
 * https://oeis.org/A331633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th distinct value among A055944(m) = m +
 *            reverse_binary(m), m = 0, 1, 2, ..., in order of first
 *            appearance (computed by scanning m with a hash set)
 * Range:     u8 n <= 78, u16 n <= 6482, u32 n <= 10810, u64 n <= 10810,
 *            u128 n <= 10810, big any n
 * Validated: standard, pass, 77 terms through n = 9285 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A331633_terms = 0;
static size_t A331633_cnt = 0, A331633_capT = 0;
static uint64_t *A331633_tab = 0;
static size_t A331633_tabsz = 0;
static uint64_t A331633_next = 0;

static size_t A331633_hash(uint64_t k, size_t mask)
{
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return (size_t)k & mask;
}

static void A331633_insert_raw(uint64_t key)
{
    size_t mask = A331633_tabsz - 1;
    size_t h = A331633_hash(key, mask);
    while (A331633_tab[h])
        h = (h + 1) & mask;
    A331633_tab[h] = key;
}

static void A331633_grow(void)
{
    size_t nsz = A331633_tabsz ? A331633_tabsz * 2 : 1024;
    free(A331633_tab);
    A331633_tab = (uint64_t *)calloc(nsz, sizeof(uint64_t));
    A331633_tabsz = nsz;
    for (size_t i = 0; i < A331633_cnt; i++)
        A331633_insert_raw(A331633_terms[i] + 1);
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    while (A331633_cnt < need)
    {
        uint64_t m = A331633_next++;
        uint64_t x = m, rev = 0;
        while (x)
        {
            rev = (rev << 1) | (x & 1);
            x >>= 1;
        }
        uint64_t v = m + rev;
        uint64_t key = v + 1;
        if ((A331633_cnt + 1) * 2 >= A331633_tabsz)
            A331633_grow();
        size_t mask = A331633_tabsz - 1;
        size_t h = A331633_hash(key, mask);
        int found = 0;
        while (A331633_tab[h])
        {
            if (A331633_tab[h] == key) { found = 1; break; }
            h = (h + 1) & mask;
        }
        if (found)
            continue;
        A331633_tab[h] = key;
        if (A331633_cnt == A331633_capT)
        {
            A331633_capT = A331633_capT ? A331633_capT * 2 : 1024;
            A331633_terms = (uint64_t *)realloc(A331633_terms, A331633_capT * sizeof(uint64_t));
        }
        A331633_terms[A331633_cnt++] = v;
    }
    return (seq_t)A331633_terms[need - 1];
}
