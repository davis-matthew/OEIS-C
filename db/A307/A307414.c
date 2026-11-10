/*
 * A307414  Numbers k such that A014285(k) and A007504(k) are coprime.
 * https://oeis.org/A307414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that gcd(A007504(k), A014285(k)) = 1; found
 *            by incremental search with running prefix sums
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9249 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static uint64_t *s307414_terms = NULL;
static size_t s307414_cnt = 0, s307414_cap = 0;
static uint64_t *s307414_pr = NULL;
static size_t s307414_pcnt = 0, s307414_pcap = 0;
static uint64_t s307414_k = 0;
static seq_t s307414_S, s307414_T;
static bool s307414_init = false;

static uint64_t s307414_next_prime(void)
{
    uint64_t c = s307414_pcnt ? s307414_pr[s307414_pcnt - 1] + 1 : 2;
    for (;; ++c) {
        bool ok = true;
        for (size_t i = 0; i < s307414_pcnt; ++i) {
            uint64_t p = s307414_pr[i];
            if (p * p > c) break;
            if (c % p == 0) { ok = false; break; }
        }
        if (ok) break;
    }
    if (s307414_pcnt == s307414_pcap) {
        s307414_pcap = s307414_pcap ? s307414_pcap * 2 : 1024;
        s307414_pr = (uint64_t *)realloc(s307414_pr, s307414_pcap * sizeof(uint64_t));
        if (!s307414_pr) abort();
    }
    s307414_pr[s307414_pcnt++] = c;
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!s307414_init) {
        s307414_S = 0;
        s307414_T = 0;
        s307414_init = true;
    }
    while (s307414_cnt < (size_t)n) {
        uint64_t p = s307414_next_prime();
        s307414_k++;
        s307414_S += (seq_t)p;
        s307414_T += (seq_t)s307414_k * (seq_t)p;
        if (gcd(s307414_S, s307414_T) == 1) {
            if (s307414_cnt == s307414_cap) {
                s307414_cap = s307414_cap ? s307414_cap * 2 : 1024;
                s307414_terms = (uint64_t *)realloc(s307414_terms, s307414_cap * sizeof(uint64_t));
                if (!s307414_terms) abort();
            }
            s307414_terms[s307414_cnt++] = s307414_k;
        }
    }
    return (seq_t)s307414_terms[n - 1];
}
