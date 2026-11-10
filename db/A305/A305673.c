/*
 * A305673  Indices i where a run of zeros starts in A305671.
 * https://oeis.org/A305673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th index i with A305671(i) = 0 and A305671(i-1) != 0;
 *            computed by scanning composites, tracking the most common sigma
 *            value and whether it is tied
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static uint64_t A305673_sigma(uint64_t m)
{
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
            }
            res *= term;
        }
    }
    if (m > 1) res *= (1 + m);
    return res;
}

seq_t a(idx_t n)
{
    static uint64_t next_m = 4;
    static uint64_t comp_idx = 0;
    static uint32_t *freq = NULL;
    static size_t freq_cap = 0;
    static uint32_t best_count = 0;
    static uint32_t tie_count = 0;
    static bool prev_zero = false;
    static seq_t *found = NULL;
    static size_t found_cnt = 0;
    static size_t found_cap = 0;

    while (found_cnt < (size_t)n) {
        uint64_t s;
        for (;;) {
            s = A305673_sigma(next_m);
            if (s > next_m + 1) break;
            next_m++;
        }
        next_m++;
        comp_idx++;

        if (s >= freq_cap) {
            size_t nc = freq_cap ? freq_cap : 1024;
            while (nc <= s) nc *= 2;
            uint32_t *p = (uint32_t *)realloc(freq, nc * sizeof(uint32_t));
            if (!p) exit(1);
            memset(p + freq_cap, 0, (nc - freq_cap) * sizeof(uint32_t));
            freq = p;
            freq_cap = nc;
        }
        uint32_t c = ++freq[s];
        if (c > best_count) {
            best_count = c;
            tie_count = 1;
        } else if (c == best_count) {
            tie_count++;
        }

        bool zero = (tie_count > 1);
        if (zero && !prev_zero) {
            if (found_cnt >= found_cap) {
                size_t nc = found_cap ? found_cap * 2 : 64;
                seq_t *p = mem_resize(seq_t, found, nc);
                if (!p) exit(1);
                found = p;
                found_cap = nc;
            }
            found[found_cnt++] = (seq_t)comp_idx;
        }
        prev_zero = zero;
    }
    return found[n - 1];
}
