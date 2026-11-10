/*
 * A382017  Record positions in A277847.
 * https://oeis.org/A382017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th strict record of A277847 (computed
 *            by scanning m = 1, 2, ... and tracking the running maximum of
 *            A277847(m))
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 939 (2026-11-10)
 * Helpers:   ipow_idx, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/odd_part.h"

static seq_t A277847_a(idx_t n) {
    if (n == 1) return 1;
    idx_t temp = n;
    seq_t result = 1;

    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
        result *= 2;
    }

    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t odd = odd_part((seq_t)(p - 1));
            seq_t power = ipow_idx((seq_t)p, exp - 1);
            seq_t contrib = odd * power + 1;
            result *= contrib;
        }
    }

    if (temp > 1) {
        seq_t odd = odd_part((seq_t)(temp - 1));
        seq_t contrib = odd + 1;
        result *= contrib;
    }

    return result;
}

static idx_t *rec_pos = NULL;
static size_t rec_cnt = 0;
static size_t rec_cap = 0;
static idx_t rec_m = 0;
static seq_t rec_best = 0;

seq_t a(idx_t n) {
    while (rec_cnt < (size_t)n) {
        rec_m++;
        seq_t v = A277847_a(rec_m);
        if (v > rec_best) {
            rec_best = v;
            if (rec_cnt == rec_cap) {
                size_t nc = rec_cap ? rec_cap * 2 : 256;
                idx_t *np = (idx_t *)realloc(rec_pos, nc * sizeof(idx_t));
                if (!np) return 0;
                rec_pos = np;
                rec_cap = nc;
            }
            rec_pos[rec_cnt++] = rec_m;
        }
    }
    return (seq_t)rec_pos[n - 1];
}
