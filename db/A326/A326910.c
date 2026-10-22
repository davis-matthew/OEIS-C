/*
 * A326910  BII-numbers of pairwise intersecting set-systems.
 * https://oeis.org/A326910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   A326910:  BII-numbers of pairwise intersecting set-systems.
   a(1)=0, a(2)=1, a(3)=2, a(4)=4, a(5)=5, a(6)=6, a(7)=8, a(8)=16, ...
   The sequence consists of all non‑negative integers n for which the
   set‑system obtained by taking the binary indices of each binary
   index of n is pairwise intersecting.
   This routine generates the sequence by direct test; it keeps a
   cache of the numbers already found.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Number of bit positions in the largest seq_t considered.
   Sufficient for the first 10 000 terms on the normal ranges. */
#define MAX_POS 128

/* Test whether the set‑system derived from num is pairwise intersecting. */
static bool pairwise_intersecting(seq_t num)
{
    /* Special case: 0 corresponds to the empty set‑system. */
    if (num == (seq_t)0)
        return true;

    /* pos_masks[i] will hold the mask of binary indices of the i‑th
       binary index of num.  The maximum number of 1‑bits in num is
       less than MAX_POS. */
    seq_t pos_masks[MAX_POS];
    size_t mask_cnt = 0;

    /* Scan bits of num, LSB first.  pos tracks the bit position (1‑based). */
    idx_t pos = 1;          /* pos ranges up to MAX_POS */
    seq_t tmp = num;
    while (tmp) {
        if (tmp & (seq_t)1) {
            /* Compute mask of binary indices of pos. */
            seq_t mask = (seq_t)0;
            idx_t t = pos;
            idx_t mpos = 1;   /* position within pos (1‑based) */
            while (t) {
                if (t & (idx_t)1)
                    mask |= ((seq_t)1 << (mpos - 1));
                mpos++;
                t >>= 1;
            }
            pos_masks[mask_cnt++] = mask;
        }
        pos++;
        tmp >>= 1;
    }

    /* Check pairwise intersection of all masks. */
    for (size_t i = 0; i < mask_cnt; ++i) {
        for (size_t j = i + 1; j < mask_cnt; ++j) {
            if ((pos_masks[i] & pos_masks[j]) == (seq_t)0)
                return false;
        }
    }
    return true;
}

/* Return the nth value of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = (seq_t)0;

    /* Grow the cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {
            /* The first term of the sequence is 0. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)0;
        } else {
            /* Find the next number that satisfies the condition. */
            do {
                ++candidate;
            } while (!pairwise_intersecting(candidate));

            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
    }

    return cache[(size_t)n - 1];
}
