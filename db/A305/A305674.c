/*
 * A305674  Indices i such that A305671(i) != A305671(i-1).
 * https://oeis.org/A305674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index i >= 2 with A305671(i) != A305671(i-1);
 *            computed by a running scan over composites with a sigma
 *            frequency table
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    static uint64_t *changes = NULL;
    static size_t nchg = 0, chg_cap = 0;
    static uint64_t comp_count = 0;
    static uint64_t next_cand = 4;
    static uint32_t *freq = NULL;
    static size_t freq_cap = 0;
    static uint64_t best_sigma = 0;
    static uint32_t best_count = 0, best_ties = 0;
    static uint64_t prev_val = 0;

    while (nchg < (size_t)n) {
        uint64_t num = next_cand++;
        uint64_t s = 1 + num;
        for (uint64_t d = 2; d * d <= num; ++d) {
            if (num % d == 0) {
                uint64_t o = num / d;
                s += d;
                if (o != d) s += o;
            }
        }
        if (s == num + 1) continue; /* prime */

        if (s >= freq_cap) {
            size_t nc = freq_cap ? freq_cap * 2 : 256;
            while (nc <= s) nc <<= 1;
            uint32_t *na = (uint32_t *)realloc(freq, nc * sizeof(uint32_t));
            if (!na) exit(1);
            memset(na + freq_cap, 0, (nc - freq_cap) * sizeof(uint32_t));
            freq = na;
            freq_cap = nc;
        }
        uint32_t c = ++freq[s];
        if (c > best_count) {
            best_count = c;
            best_sigma = s;
            best_ties = 1;
        } else if (c == best_count) {
            best_ties++;
        }
        uint64_t val = (best_ties == 1) ? best_sigma : 0;
        comp_count++;
        if (comp_count >= 2 && val != prev_val) {
            if (nchg >= chg_cap) {
                size_t nc = chg_cap ? chg_cap * 2 : 64;
                uint64_t *na = (uint64_t *)realloc(changes, nc * sizeof(uint64_t));
                if (!na) exit(1);
                changes = na;
                chg_cap = nc;
            }
            changes[nchg++] = comp_count;
        }
        prev_val = val;
    }
    return (seq_t)changes[(size_t)n - 1];
}
