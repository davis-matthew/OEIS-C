/*
 * A305675  Run lengths of successive equal terms in A305671.
 * https://oeis.org/A305675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal consecutive terms in
 *            A305671 (A305671(k) = most common sigma value among composites
 *            up to the k-th composite, 0 on ties); computed by streaming
 *            A305671 and counting runs
 * Range:     u8 n <= 18, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t A305675_sigma(uint64_t num)
{
    uint64_t s = 1 + num;
    for (uint64_t d = 2; d * d <= num; ++d) {
        if (num % d == 0) {
            uint64_t o = num / d;
            s += d;
            if (o != d) s += o;
        }
    }
    return s;
}

static bool A305675_is_composite(uint64_t m)
{
    if (m < 4) return false;
    for (uint64_t d = 2; d * d <= m; ++d)
        if (m % d == 0) return true;
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *runs = NULL;
    static size_t runs_cnt = 0, runs_cap = 0;
    static uint32_t *freq = NULL;
    static size_t freq_cap = 0;
    static uint64_t next_cand = 4;
    static uint64_t best_sigma = 0, best_count = 0, best_ties = 0;
    static bool started = false;
    static uint64_t cur = 0, curlen = 0;

    while ((idx_t)runs_cnt < n) {
        while (!A305675_is_composite(next_cand)) next_cand++;
        uint64_t sg = A305675_sigma(next_cand);
        next_cand++;
        if (sg >= freq_cap) {
            size_t nc = freq_cap ? freq_cap * 2 : 256;
            while (nc <= sg) nc <<= 1;
            uint32_t *nf = (uint32_t *)realloc(freq, nc * sizeof(uint32_t));
            if (!nf) exit(1);
            memset(nf + freq_cap, 0, (nc - freq_cap) * sizeof(uint32_t));
            freq = nf;
            freq_cap = nc;
        }
        uint64_t c = ++freq[sg];
        if (c > best_count) {
            best_count = c;
            best_sigma = sg;
            best_ties = 1;
        } else if (c == best_count) {
            best_ties++;
        }
        uint64_t v = (best_ties == 1) ? best_sigma : 0;

        if (!started) {
            started = true;
            cur = v;
            curlen = 1;
        } else if (v == cur) {
            curlen++;
        } else {
            if (runs_cnt >= runs_cap) {
                size_t nc = runs_cap ? runs_cap * 2 : 64;
                uint64_t *nr = (uint64_t *)realloc(runs, nc * sizeof(uint64_t));
                if (!nr) exit(1);
                runs = nr;
                runs_cap = nc;
            }
            runs[runs_cnt++] = curlen;
            cur = v;
            curlen = 1;
        }
    }
    return (seq_t)runs[n - 1];
}
