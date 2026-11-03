/*
 * A243935  Numbers m such that 5 divides A000041(m).
 * https://oeis.org/A243935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 901 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243935: numbers m such that 5 divides the partition number p(m). 
   The function computes p(m) modulo 5 using Euler's pentagonal theorem
   and collects the indices where the remainder is 0.  The array is
   built lazily so that calls with arbitrary n in any order are fast. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t partition_mod5(size_t m, seq_t *mods) {
    int sum = 0;
    for (int k = 1; ; ++k) {
        size_t g = (size_t)k * ((size_t)3 * (size_t)k - 1u) / 2;
        if (g > m) break;
        int sgn = (k & 1) ? 1 : -1;
        sum += sgn * (int)mods[m - g];
        size_t h = (size_t)k * ((size_t)3 * (size_t)k + 1u) / 2;
        if (h <= m) sum += sgn * (int)mods[m - h];
    }
    int rem = sum % 5;
    if (rem < 0) rem += 5;
    return (seq_t)rem;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset 1: a(1) is first term */

    static seq_t *pmods = NULL;
    static size_t p_used = 0, p_cap = 0;
    static seq_t *hits = NULL;
    static size_t hit_used = 0, hit_cap = 0;

    /* initialise pmods with p(0)=1 */
    if (p_cap == 0) {
        p_cap = 256;
        pmods = mem_new(seq_t, p_cap);
        if (!pmods) return 0;
    }
    if (p_used == 0) {
        pmods[0] = (seq_t)1;
        p_used = 1;
    }

    /* grow hits storage if needed for the final value */
    if (hit_cap < 256) {
        hit_cap = 256;
        hits = mem_new(seq_t, hit_cap);
        if (!hits) return 0;
    }

    while (hit_used < (size_t)n) {
        size_t m = p_used;                 /* next m to compute */
        seq_t rem = partition_mod5(m, pmods);

        /* grow pmods if necessary */
        if (p_used + 1 > p_cap) {
            size_t newcap = p_cap ? 2 * p_cap : 256;
            seq_t *tmp = mem_resize(seq_t, pmods, newcap);
            if (!tmp) return 0;
            pmods = tmp;
            p_cap = newcap;
        }
        pmods[p_used] = rem;
        ++p_used;

        if (rem == 0) {
            if (hit_used + 1 > hit_cap) {
                size_t newhitcap = hit_cap ? 2 * hit_cap : 256;
                seq_t *tmp = mem_resize(seq_t, hits, newhitcap);
                if (!tmp) return 0;
                hits = tmp;
                hit_cap = newhitcap;
            }
            hits[hit_used] = (seq_t)m;
            ++hit_used;
        }
    }

    return hits[n - 1];
}
