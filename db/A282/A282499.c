/*
 * A282499  Expansion of (Sum_{k = i^j, i>=1, j>=2, excluding duplicates}
 *          x^k)^3.
 * https://oeis.org/A282499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (a,b,c) of perfect powers
 *            (i>=1, j>=2) such that a+b+c = n
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Global state */
static idx_t current_limit = 0;
static seq_t *ways_arr   = NULL;

/* Rebuild tables up to maxN */
static void rebuild(idx_t maxN)
{
    /* 1. Build list of unique perfect powers <= maxN */
    size_t pp_cap = 64;
    idx_t *pp = (idx_t*)malloc(pp_cap * sizeof(idx_t));
    size_t pp_len = 0;

    bool *seen = (bool*)calloc((size_t)maxN + 1, sizeof(bool));

    /* include 1 */
    if (maxN >= 1) {
        seen[1] = true;
        pp[pp_len++] = 1;
    }

    for (idx_t base = 2; base <= maxN; ++base) {
        uint64_t power = (uint64_t)base * (uint64_t)base;
        while (power <= maxN) {
            idx_t val = (idx_t)power;
            if (!seen[val]) {
                if (pp_len == pp_cap) {
                    pp_cap <<= 1;
                    pp = (idx_t*)realloc(pp, pp_cap * sizeof(idx_t));
                }
                seen[val] = true;
                pp[pp_len++] = val;
            }
            if (power > (uint64_t)maxN / (uint64_t)base) break;
            power *= (uint64_t)base;
        }
    }
    free(seen);

    /* 2. First convolution: number of ordered pairs summing to each value */
    seq_t *conv2 = (seq_t*)calloc((size_t)maxN + 1, sizeof(seq_t));

    for (size_t i = 0; i < pp_len; ++i) {
        idx_t a = pp[i];
        for (size_t j = 0; j < pp_len; ++j) {
            idx_t b = pp[j];
            idx_t sum = a + b;
            if (sum <= maxN) {
                conv2[sum] += (seq_t)1;
            }
        }
    }

    /* 3. Second convolution: add third perfect power */
    seq_t *new_ways = (seq_t*)calloc((size_t)maxN + 1, sizeof(seq_t));

    for (size_t k = 0; k < pp_len; ++k) {
        idx_t c = pp[k];
        for (idx_t s = c; s <= maxN; ++s) {
            new_ways[s] += conv2[s - c];
        }
    }

    free(conv2);
    free(pp);

    /* 4. Store results */
    if (ways_arr) free(ways_arr);
    ways_arr = new_ways;
    current_limit = maxN;
}

seq_t a(idx_t n)
{
    if (!ways_arr || n > current_limit) rebuild(n);
    return ways_arr[n];
}
