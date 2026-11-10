/*
 * A347520  A053392 with duplicates removed.
 * https://oeis.org/A347520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct value (in order of first appearance) of
 *            A053392(m), m = 0,1,2,...; A053392(m) = concatenation of sums of
 *            consecutive digit pairs of m (0 if m<10). Computed by scan with
 *            a hash set
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A347520_f(uint64_t m)
{
    if (m < 10) return 0;
    uint8_t d[20];
    int len = 0;
    while (m > 0) { d[len++] = (uint8_t)(m % 10); m /= 10; }
    uint64_t r = 0;
    for (int i = len - 1; i >= 1; --i) {
        unsigned s = d[i] + d[i - 1];
        r = r * (s < 10 ? 10 : 100) + s;
    }
    return r;
}

static uint64_t *A347520_terms = 0;
static size_t A347520_cnt = 0, A347520_cap = 0;
static uint64_t *A347520_tab = 0;   /* stores value+1, 0 = empty */
static size_t A347520_tbits = 0;
static uint64_t A347520_next = 0;

static size_t A347520_slot(uint64_t v, size_t bits)
{
    return (size_t)((v * 0x9E3779B97F4A7C15ULL) >> (64 - bits));
}

static void A347520_insert_raw(uint64_t v)
{
    size_t mask = ((size_t)1 << A347520_tbits) - 1;
    size_t s = A347520_slot(v, A347520_tbits);
    while (A347520_tab[s]) s = (s + 1) & mask;
    A347520_tab[s] = v + 1;
}

static void A347520_grow(void)
{
    size_t nb = A347520_tbits ? A347520_tbits + 1 : 12;
    free(A347520_tab);
    A347520_tab = (uint64_t *)calloc((size_t)1 << nb, sizeof(uint64_t));
    A347520_tbits = nb;
    for (size_t i = 0; i < A347520_cnt; i++) A347520_insert_raw(A347520_terms[i]);
}

seq_t a(idx_t n)
{
    while (A347520_cnt <= (size_t)n) {
        uint64_t v = A347520_f(A347520_next++);
        if (!A347520_tab) A347520_grow();
        size_t mask = ((size_t)1 << A347520_tbits) - 1;
        size_t s = A347520_slot(v, A347520_tbits);
        int found = 0;
        while (A347520_tab[s]) {
            if (A347520_tab[s] == v + 1) { found = 1; break; }
            s = (s + 1) & mask;
        }
        if (found) continue;
        if (A347520_cnt == A347520_cap) {
            A347520_cap = A347520_cap ? A347520_cap * 2 : 1024;
            A347520_terms = (uint64_t *)realloc(A347520_terms, A347520_cap * sizeof(uint64_t));
        }
        A347520_terms[A347520_cnt++] = v;
        if (A347520_cnt * 2 > ((size_t)1 << A347520_tbits)) A347520_grow();
        else A347520_tab[s] = v + 1;
    }
    return (seq_t)A347520_terms[(size_t)n];
}
