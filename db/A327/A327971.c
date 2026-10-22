/*
 * A327971  Bitwise XOR of trajectories of rule 30 and its mirror image, rule
 *          86, when both are started from a lone 1 cell: a(n) = A110240(n)
 *          XOR A265281(n).
 * https://oeis.org/A327971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A110240(n) XOR A265281(n) (Rule 30 and its mirror Rule 86
 *            cellular‑automaton starting from a single 1 cell.) The function
 *            caches all computed values so that calls in any order are
 *            efficient
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   build_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_val.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* current state of the two automata */
    static uint8_t *cur30   = NULL;
    static size_t cur30_len = 0;
    static uint8_t *cur86   = NULL;
    static size_t cur86_len = 0;
    static idx_t cur_n      = 0;

    /* initialise */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* n = 0, starting row is a single 1 for both automata */
        cur30_len = 1;
        cur30 = (uint8_t*)malloc(cur30_len * sizeof(uint8_t));
        cur30[0] = 1;
        cur86_len = 1;
        cur86 = (uint8_t*)malloc(cur86_len * sizeof(uint8_t));
        cur86[0] = 1;

        /* value for a(0) */
        cache[0] = 0;          /* 1 XOR 1 = 0 */
        cache_len = 1;
        cur_n      = 0;
    }

    /* grow cache if necessary */
    while (cur_n < n) {
        size_t old_len = cur30_len;          /* cur30_len == cur86_len by construction */
        int   old_n    = (int)((old_len - 1) / 2);
        size_t new_len = old_len + 2;
        int   new_n    = old_n + 1;

        /* allocate space for the next row */
        uint8_t *next30 = (uint8_t*)malloc(new_len * sizeof(uint8_t));
        uint8_t *next86 = (uint8_t*)malloc(new_len * sizeof(uint8_t));

        /* compute next row for both automata */
        for (size_t j = 0; j < new_len; ++j) {
            int p_new      = (int)j - new_n;        /* position in new row   */
            int left_idx   = p_new - 1 + old_n;     /* left neighbor index   */
            int center_idx = p_new     + old_n;     /* center neighbor index*/
            int right_idx  = p_new + 1 + old_n;     /* right neighbor index  */

            uint8_t left  = (left_idx  >= 0 && (size_t)left_idx  < old_len) ? cur30[left_idx]  : 0;
            uint8_t center= (center_idx>= 0 && (size_t)center_idx< old_len) ? cur30[center_idx] : 0;
            uint8_t right = (right_idx >= 0 && (size_t)right_idx < old_len) ? cur30[right_idx]  : 0;
            next30[j] = left ^ (center | right);

            left   = (left_idx  >= 0 && (size_t)left_idx  < old_len) ? cur86[left_idx]  : 0;
            center = (center_idx>= 0 && (size_t)center_idx< old_len) ? cur86[center_idx] : 0;
            right  = (right_idx >= 0 && (size_t)right_idx < old_len) ? cur86[right_idx]  : 0;
            next86[j] = right ^ (center | left);
        }

        /* construct integer values of the two rows */
        seq_t val30 = build_val(next30, new_len);
        seq_t val86 = build_val(next86, new_len);
        seq_t aval  = val30 ^ val86;

        /* store the new a(n) value */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = aval;
        ++cache_len;
        ++cur_n;

        /* free previous row and continue */
        free(cur30);
        free(cur86);
        cur30 = next30;
        cur30_len = new_len;
        cur86 = next86;
        cur86_len = new_len;
    }

    return cache[n];
}
