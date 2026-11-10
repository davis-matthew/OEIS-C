/*
 * A348782  Records in A342585.
 * https://oeis.org/A348782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th left-to-right record (strict new maximum) of the
 *            inventory sequence A342585; computed by direct simulation of
 *            A342585
 * Range:     u8 n <= 87, u16 n <= 955, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 91 terms through n = 8915 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static uint64_t *freq = NULL;
    static size_t freq_cap = 0;
    static uint64_t seg_i = 0;
    static uint64_t *rec = NULL;
    static size_t rec_cnt = 0, rec_cap = 0;
    static bool have_max = false;
    static uint64_t maxv = 0;

    if (!freq) {
        freq_cap = 1024;
        freq = (uint64_t *)calloc(freq_cap, sizeof(uint64_t));
    }
    while (rec_cnt < (size_t)n) {
        uint64_t cnt = (seg_i < freq_cap) ? freq[seg_i] : 0;
        if (cnt >= freq_cap) {
            size_t nc = freq_cap;
            while (nc <= cnt) nc <<= 1;
            uint64_t *t = (uint64_t *)realloc(freq, nc * sizeof(uint64_t));
            for (size_t i = freq_cap; i < nc; ++i) t[i] = 0;
            freq = t;
            freq_cap = nc;
        }
        ++freq[cnt];
        if (cnt == 0) seg_i = 0; else ++seg_i;
        if (!have_max || cnt > maxv) {
            have_max = true;
            maxv = cnt;
            if (rec_cnt == rec_cap) {
                rec_cap = rec_cap ? rec_cap * 2 : 1024;
                rec = (uint64_t *)realloc(rec, rec_cap * sizeof(uint64_t));
            }
            rec[rec_cnt++] = cnt;
        }
    }
    return (seq_t)rec[(size_t)(n - 1)];
}
