/*
 * A347305  Indices of records in A342585.
 * https://oeis.org/A347305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record (strict new maximum) in A342585;
 *            computed by generating A342585 incrementally with a frequency
 *            table
 * Range:     u8 n <= 20, u16 n <= 281, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 69 terms through n = 9300 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static idx_t *freq = NULL;
    static size_t freq_cap = 0;
    static idx_t *rec = NULL;
    static size_t rec_cnt = 0, rec_cap = 0;
    static idx_t pos = 0;
    static idx_t seg_i = 0;
    static idx_t cur_max = 0;
    static bool have_max = false;

    if (!freq) {
        freq_cap = 64;
        freq = (idx_t*)malloc(sizeof(idx_t) * freq_cap);
        for (size_t i = 0; i < freq_cap; ++i) freq[i] = 0;
    }
    if (!rec) {
        rec_cap = 64;
        rec = (idx_t*)malloc(sizeof(idx_t) * rec_cap);
    }

    while (rec_cnt < (size_t)n) {
        idx_t cnt = (seg_i < (idx_t)freq_cap) ? freq[seg_i] : 0;
        ++pos;
        if (cnt >= (idx_t)freq_cap) {
            size_t nc = freq_cap;
            while (nc <= (size_t)cnt) nc <<= 1;
            freq = (idx_t*)realloc(freq, sizeof(idx_t) * nc);
            for (size_t i = freq_cap; i < nc; ++i) freq[i] = 0;
            freq_cap = nc;
        }
        ++freq[cnt];
        if (!have_max || cnt > cur_max) {
            have_max = true;
            cur_max = cnt;
            if (rec_cnt >= rec_cap) {
                rec_cap *= 2;
                rec = (idx_t*)realloc(rec, sizeof(idx_t) * rec_cap);
            }
            rec[rec_cnt++] = pos;
        }
        if (cnt == 0) seg_i = 0;
        else ++seg_i;
    }
    return (seq_t)rec[(size_t)(n - 1)];
}
