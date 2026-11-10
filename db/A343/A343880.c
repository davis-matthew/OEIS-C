/*
 * A343880  Positions of 0's in A342585.
 * https://oeis.org/A343880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 0 in A342585; computed by simulating
 *            the inventory sequence A342585 (counts only, no sequence
 *            storage) until n zeros are found
 * Range:     u8 n <= 20, u16 n <= 283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 8562 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static idx_t *freq = NULL;
    static size_t freq_cap = 0;
    static idx_t *zeros = NULL;
    static size_t zeros_cnt = 0, zeros_cap = 0;
    static idx_t pos = 0;
    static size_t seg_i = 0;

    if (!freq) {
        freq_cap = 64;
        freq = (idx_t*)calloc(freq_cap, sizeof(idx_t));
        zeros_cap = 64;
        zeros = (idx_t*)malloc(zeros_cap * sizeof(idx_t));
    }

    while (zeros_cnt < (size_t)n) {
        idx_t cnt = (seg_i < freq_cap) ? freq[seg_i] : 0;
        if ((size_t)cnt >= freq_cap) {
            size_t new_cap = freq_cap;
            while (new_cap <= (size_t)cnt) new_cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(freq, new_cap * sizeof(idx_t));
            for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
            freq = tmp;
            freq_cap = new_cap;
        }
        ++freq[(size_t)cnt];
        ++pos;
        if (cnt == 0) {
            if (zeros_cnt >= zeros_cap) {
                zeros_cap <<= 1;
                zeros = (idx_t*)realloc(zeros, zeros_cap * sizeof(idx_t));
            }
            zeros[zeros_cnt++] = pos;
            seg_i = 0;
        } else {
            ++seg_i;
        }
    }
    return (seq_t)zeros[(size_t)(n - 1)];
}
