/*
 * A255651  Partial sums of A255485.
 * https://oeis.org/A255651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=0..n} A255485(k), where A255485(k) = number of odd
 *            coefficients of (1+x+x^2+x^4)^k, computed by iterating the
 *            polynomial mod 2 as a bitset
 * Range:     u8 n <= 19, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static idx_t cur_n = 0;
    static seq_t *sums = NULL;
    static size_t sums_cap = 0;
    static uint64_t *bits = NULL;
    static size_t bits_cap = 0;
    static size_t wc = 0;

    if (!sums) {
        sums_cap = 16;
        sums = mem_new(seq_t, sums_cap);
        if (!sums) abort();
        sums[0] = (seq_t)1;
        bits_cap = 4;
        bits = (uint64_t *)calloc(bits_cap, sizeof(uint64_t));
        if (!bits) abort();
        bits[0] = 1ULL;
        wc = 1;
        cur_n = 0;
    }

    while (cur_n < n) {
        idx_t next = cur_n + 1;
        size_t new_wc = (size_t)((4 * next) >> 6) + 1;
        if (new_wc > bits_cap) {
            size_t nc = bits_cap * 2;
            if (nc < new_wc) nc = new_wc;
            uint64_t *t = (uint64_t *)realloc(bits, nc * sizeof(uint64_t));
            if (!t) abort();
            bits = t;
            bits_cap = nc;
        }
        for (size_t i = wc; i < new_wc; ++i) bits[i] = 0;

        unsigned cnt = 0;
        for (size_t ii = new_wc; ii-- > 0;) {
            uint64_t v = bits[ii];
            uint64_t w = v ^ (v << 1) ^ (v << 2) ^ (v << 4);
            if (ii > 0) {
                uint64_t p = bits[ii - 1];
                w ^= (p >> 63) ^ (p >> 62) ^ (p >> 60);
            }
            bits[ii] = w;
            cnt += (unsigned)__builtin_popcountll(w);
        }
        wc = new_wc;

        if ((size_t)next + 1 > sums_cap) {
            size_t nl = sums_cap * 2;
            if (nl < (size_t)next + 2) nl = (size_t)next + 2;
            seq_t *t = mem_resize(seq_t, sums, nl);
            if (!t) abort();
            sums = t;
            sums_cap = nl;
        }
        sums[next] = sums[cur_n] + (seq_t)cnt;
        cur_n = next;
    }
    return sums[n];
}
