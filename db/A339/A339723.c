/*
 * A339723  Start with a(1)=1. Thereafter, to get a(n), write a(n-1) in
 *          binary, write down all its run lengths, and concatenate them in
 *          binary, getting k. Then a(n) = k unless k is already in the
 *          sequence, in which case a(n) = smallest missing number.
 * https://oeis.org/A339723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1. Recurrence (n > 1): Write a(n‑1) in binary, find the
 *            run lengths of consecutive identical bits, concatenate those run
 *            lengths in binary to obtain k. If k has not appeared earlier in
 *            the sequence let a(n) = k; otherwise a(n) is the smallest
 *            positive integer not yet in the sequence
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A339723: a(1) = 1; k = the run lengths of a(n-1) in binary, each written in
 * binary and concatenated; a(n) = k unless k already occurred, else the
 * smallest number not yet in the sequence. Generated in order with exact
 * 64-bit values (k has at most as many bits as a(n-1)) and a hash set of the
 * values used. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static uint64_t *hs = NULL;               /* open addressing; 0 = empty (values are >= 1) */
static size_t hcap = 0, hcnt = 0;
static uint64_t smallest = 1;

static size_t slot(uint64_t v) { return (size_t)((v * UINT64_C(0x9E3779B97F4A7C15)) >> 7) & (hcap - 1); }

static int has(uint64_t v)
{
    for (size_t i = slot(v);; i = (i + 1) & (hcap - 1)) {
        if (hs[i] == v) return 1;
        if (!hs[i]) return 0;
    }
}

static void add(uint64_t v)
{
    if (2 * (hcnt + 1) > hcap) {
        uint64_t *old = hs;
        size_t oc = hcap;
        hcap = hcap ? 2 * hcap : 1024;
        hs = (uint64_t *)calloc(hcap, sizeof *hs);
        if (!hs) abort();
        hcnt = 0;
        for (size_t i = 0; i < oc; ++i) if (old[i]) add(old[i]);
        free(old);
    }
    size_t i = slot(v);
    while (hs[i]) i = (i + 1) & (hcap - 1);
    hs[i] = v;
    ++hcnt;
}

static uint64_t runs_code(uint64_t x)
{
    int bits = 64 - __builtin_clzll(x), i = bits - 1;
    uint64_t k = 0;
    while (i >= 0) {
        int b = (x >> i) & 1, r = 0;
        while (i >= 0 && (int)((x >> i) & 1) == b) { ++r; --i; }
        int rb = 32 - __builtin_clz((unsigned)r);
        k = (k << rb) | (uint64_t)r;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        uint64_t v = term_len ? runs_code(terms[term_len - 1]) : 1;
        if (term_len && has(v)) {
            while (has(smallest)) ++smallest;
            v = smallest;
        }
        add(v);
        if (term_len == term_cap) {
            term_cap = term_cap ? 2 * term_cap : 1024;
            uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
            if (!t) abort();
            terms = t;
        }
        terms[term_len++] = v;
    }
    return (seq_t)terms[n - 1];
}
