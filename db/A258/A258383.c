/*
 * A258383  Run lengths of consecutive identical terms in A062234.
 * https://oeis.org/A258383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th maximal run of equal consecutive terms
 *            in A062234, where A062234(k) = 2*prime(k) - prime(k+1); computed
 *            by scanning terms and recording run lengths
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8650 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *r383_pr = NULL;
static size_t r383_pc = 0, r383_pcap = 0;
static uint64_t *r383_runs = NULL;
static size_t r383_nr = 0, r383_rcap = 0;
static uint64_t r383_k = 0;       /* terms processed */
static int64_t r383_cur = 0;
static uint64_t r383_len = 0;

static void r383_pushp(uint32_t p)
{
    if (r383_pc >= r383_pcap) {
        size_t nc = r383_pcap ? r383_pcap * 2 : 64;
        uint32_t *t = (uint32_t *)realloc(r383_pr, nc * sizeof(uint32_t));
        if (!t) abort();
        r383_pr = t;
        r383_pcap = nc;
    }
    r383_pr[r383_pc++] = p;
}

static void r383_ensure(size_t need)
{
    if (r383_pc == 0) r383_pushp(2);
    if (r383_pc == 1 && need > 1) r383_pushp(3);
    while (r383_pc < need) {
        uint32_t c = r383_pr[r383_pc - 1] + 2;
        for (;; c += 2) {
            int ok = 1;
            for (size_t i = 1; i < r383_pc; ++i) {
                uint32_t p = r383_pr[i];
                if ((uint64_t)p * p > c) break;
                if (c % p == 0) { ok = 0; break; }
            }
            if (ok) break;
        }
        r383_pushp(c);
    }
}

static void r383_pushrun(uint64_t l)
{
    if (r383_nr >= r383_rcap) {
        size_t nc = r383_rcap ? r383_rcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(r383_runs, nc * sizeof(uint64_t));
        if (!t) abort();
        r383_runs = t;
        r383_rcap = nc;
    }
    r383_runs[r383_nr++] = l;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (r383_nr < (size_t)n) {
        r383_ensure((size_t)r383_k + 2);
        int64_t v = 2 * (int64_t)r383_pr[r383_k] - (int64_t)r383_pr[r383_k + 1];
        r383_k++;
        if (r383_len == 0) { r383_cur = v; r383_len = 1; }
        else if (v == r383_cur) r383_len++;
        else { r383_pushrun(r383_len); r383_cur = v; r383_len = 1; }
    }
    return (seq_t)r383_runs[n - 1];
}
