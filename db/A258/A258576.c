/*
 * A258576  a(n) = A085909(n) - prime(n).
 * https://oeis.org/A258576
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A085909(n) - prime(n); A085909(n) is found by search:
 *            smallest prime p > prime(n) whose gap to the next prime equals
 *            prime(n+1)-prime(n)
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 958 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"

static uint32_t A258576_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!pr) abort();
        pr[sz++] = 2;
    }
    while (sz < (size_t)k) {
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)p) break;
            if (p % q == 0) { ok = false; break; }
        }
        if (!ok) { cand += 2; continue; }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    uint32_t p_n = A258576_get_prime(n);
    uint32_t p_np1 = A258576_get_prime(n + 1);
    uint64_t diff = (uint64_t)(p_np1 - p_n);

    uint64_t p = next_prime((uint64_t)p_n + 1);
    for (;;) {
        uint64_t q = next_prime(p + 1);
        if (q - p == diff)
            return (seq_t)(p - (uint64_t)p_n);
        p = q;
    }
}
