/*
 * A263749  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -5 to 5 places and every three consecutive
 *          elements having its maximum within 5 of its minimum.
 * https://oeis.org/A263749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n arrays of permutations of 0..n-1 with
 *            each element moved by at most 5 places and every three
 *            consecutive elements having its maximum within 5 of its minimum
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   mask_from_code, rem1_from_code, rem2_from_code
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mask_from_code.h"
#include "../utils/rem1_from_code.h"
#include "../utils/rem2_from_code.h"

#define K 5
#define MASK_BITS (2 * K + 1)
#define FULL_MASK ((1u << MASK_BITS) - 1)
#define STATE_BITS 19
#define STATE_CAP (1u << STATE_BITS)
#define SENTINEL 0xFu  /* 4-bit sentinel value */

static inline unsigned int encode_state(uint32_t mask, uint8_t rem1, uint8_t rem2)
{
    return (mask << 8) | ( (uint32_t)rem1 << 4 ) | rem2;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0; /* offset is 1 */

    /* Allocate DP arrays */
    seq_t *cur_counts = (seq_t *)calloc(STATE_CAP, sizeof(seq_t));
    seq_t *nxt_counts = (seq_t *)calloc(STATE_CAP, sizeof(seq_t));
    if (!cur_counts || !nxt_counts)
        return (seq_t)0; /* allocation failure */

    unsigned int *cur_vec = (unsigned int *)malloc(STATE_CAP * sizeof(unsigned int));
    unsigned int *nxt_vec = (unsigned int *)malloc(STATE_CAP * sizeof(unsigned int));
    if (!cur_vec || !nxt_vec) {
        free(cur_counts); free(nxt_counts);
        return (seq_t)0;
    }
    size_t cur_len = 0, nxt_len = 0;

    /* initial mask: values <0 or >=n are marked used (bit 1) */
    uint32_t init_mask = 0;
    for (int j = 0; j < MASK_BITS; ++j) {
        int val = -K + j; /* value corresponding to bit j at pos 0 */
        if (val < 0 || val >= (int)n)
            init_mask |= (1u << j);
    }

    unsigned int init_code = encode_state(init_mask, SENTINEL, SENTINEL);
    cur_counts[init_code] = (seq_t)1;
    cur_vec[cur_len++] = init_code;

    /* DP over positions */
    for (idx_t pos = 0; pos < n; ++pos) {
        nxt_len = 0;

        for (size_t ii = 0; ii < cur_len; ++ii) {
            unsigned int code = cur_vec[ii];
            seq_t ways = cur_counts[code];
            if (ways == (seq_t)0)
                continue;

            uint32_t mask = mask_from_code(code);
            uint8_t rem1_code = rem1_from_code(code);
            uint8_t rem2_code = rem2_from_code(code);

            for (int j = 0; j < MASK_BITS; ++j) {
                /* skip if value already used or out of bounds */
                if ((mask >> j) & 1u)
                    continue;

                int val = (int)pos - K + j; /* absolute value at this position */
                if (val < 0 || val >= (int)n)
                    continue;

                /* triple constraint for pos >= 2 and when rem2 is present */
                if (pos >= 2 && rem2_code != SENTINEL) {
                    int rem1_offset = (int)rem1_code - 6; /* offset of last1 relative to pos */
                    int rem2_offset = (int)rem2_code - 7; /* offset of last2 relative to pos */
                    int v_prev1 = (int)pos + rem1_offset;
                    int v_prev2 = (int)pos + rem2_offset;

                    int mx = v_prev1;
                    if (v_prev2 > mx) mx = v_prev2;
                    if (val > mx) mx = val;
                    int mn = v_prev1;
                    if (v_prev2 < mn) mn = v_prev2;
                    if (val < mn) mn = val;
                    if (mx - mn > 5)
                        continue; /* violates local constraint */
                }

                /* mark value as used and shift window */
                uint32_t m1 = mask | (1u << j);
                uint32_t m2 = m1 >> 1; /* shift right: old bit j+1 becomes new bit j */
                uint32_t new_bit = ( (pos + K + 1) >= n ) ? 1u : 0u;
                uint32_t new_mask = m2 | (new_bit << (MASK_BITS - 1));

                /* compute new rem1 and rem2 for next position (pos+1) */
                int new_rem1_offset = j - (K + 1);          /* offset of new last1 */
                uint8_t new_rem1_code = (uint8_t)(new_rem1_offset + 6);

                uint8_t new_rem2_code;
                if (pos == 0) {              /* after first position, rem2 stays sentinel */
                    new_rem2_code = SENTINEL;
                } else {
                    if (rem1_code == SENTINEL) {   /* safety, should not happen */
                        new_rem2_code = SENTINEL;
                    } else {
                        int old_rem1_offset = (int)rem1_code - 6;
                        int new_rem2_offset = old_rem1_offset - 1;
                        new_rem2_code = (uint8_t)(new_rem2_offset + 7);
                    }
                }

                unsigned int new_code = encode_state(new_mask, new_rem1_code, new_rem2_code);
                if (nxt_counts[new_code] == (seq_t)0)
                    nxt_vec[nxt_len++] = new_code;
                nxt_counts[new_code] += ways;
            }

            /* clear current count to reuse array as next_counts later */
            cur_counts[code] = (seq_t)0;
        }

        /* swap count arrays */
        seq_t *tmp_counts = cur_counts;
        cur_counts = nxt_counts;
        nxt_counts = tmp_counts;

        /* swap vector buffers */
        unsigned int *tmp_vec = cur_vec;
        cur_vec = nxt_vec;
        nxt_vec = tmp_vec;
        cur_len = nxt_len;
    }

    /* sum results where all values are used (full mask) */
    seq_t result = (seq_t)0;
    for (size_t ii = 0; ii < cur_len; ++ii) {
        unsigned int code = cur_vec[ii];
        if (mask_from_code(code) == FULL_MASK)
            result += cur_counts[code];
    }

    free(cur_counts);
    free(nxt_counts);
    free(cur_vec);
    free(nxt_vec);

    return result;
}
