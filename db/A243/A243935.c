/*
 * A243935  Numbers m such that 5 divides A000041(m).
 * https://oeis.org/A243935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   partition_mod5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243935: numbers m such that 5 divides the partition number p(m). 
   The function computes p(m) modulo 5 using Euler's pentagonal theorem
   and collects the indices where the remainder is 0.  The array is
   built lazily so that calls with arbitrary n in any order are fast. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/partition_mod5.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset 1: a(1) is first term */

    static seq_t *pmods = NULL;
    static size_t p_used = 0, p_cap = 0;
    static seq_t *hits = NULL;
    static size_t hit_used = 0, hit_cap = 0;

    /* initialise pmods with p(0)=1 */
    if (p_cap == 0) {
        p_cap = 256;
        pmods = (seq_t *)malloc(p_cap * sizeof(seq_t));
        if (!pmods) return 0;
    }
    if (p_used == 0) {
        pmods[0] = (seq_t)1;
        p_used = 1;
    }

    /* grow hits storage if needed for the final value */
    if (hit_cap < 256) {
        hit_cap = 256;
        hits = (seq_t *)malloc(hit_cap * sizeof(seq_t));
        if (!hits) return 0;
    }

    while (hit_used < (size_t)n) {
        size_t m = p_used;                 /* next m to compute */
        seq_t rem = partition_mod5(m, pmods);

        /* grow pmods if necessary */
        if (p_used + 1 > p_cap) {
            size_t newcap = p_cap ? 2 * p_cap : 256;
            seq_t *tmp = (seq_t *)realloc(pmods, newcap * sizeof(seq_t));
            if (!tmp) return 0;
            pmods = tmp;
            p_cap = newcap;
        }
        pmods[p_used] = rem;
        ++p_used;

        if (rem == 0) {
            if (hit_used + 1 > hit_cap) {
                size_t newhitcap = hit_cap ? 2 * hit_cap : 256;
                seq_t *tmp = (seq_t *)realloc(hits, newhitcap * sizeof(seq_t));
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
