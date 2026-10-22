/*
 * A319698  Filter sequence combining A003557(n) [n divided by largest
 *          squarefree divisor of n] with A319697(n) [sum of even squarefree
 *          divisors of n].
 * https://oeis.org/A319698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   RG-transform mapping each distinct pair (A003557(n), A319697(n))
 *            to the 1‑based index of its first occurrence
 * Range:     u8 n <= 601, u16 n <= 9811, u32 n <= 9811, u64 n <= 9811,
 *            u128 n <= 9811, big any n
 * Helpers:   compute_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compute_pair.h"

/* Helper struct to store a pair (odd part, even‑squarefree sum). */
typedef struct {
    seq_t x; /* A003557(n) = n / rad(n) (odd part of n) */
    seq_t y; /* A319697(n) = sum of even squarefree divisors of n */
} Pair;

/* Dynamic array of seen pairs (RG mapping). */
static Pair *pairs = NULL;
static size_t pair_cnt = 0;
static size_t pair_cap = 0;

static void grow_pair_array(void)
{
    if (pair_cnt == pair_cap) {
        size_t new_cap = pair_cap ? pair_cap * 2 : 8;
        Pair *tmp = (Pair *)realloc(pairs, new_cap * sizeof(Pair));
        if (!tmp) exit(EXIT_FAILURE);
        pairs = tmp;
        pair_cap = new_cap;
    }
}

/* Cache of computed sequence values: cache_vals[i] == a(i+1). */
static seq_t *cache_vals = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void grow_cache_array(void)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache_vals = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out-of-range guard */

    while (cache_size < n) {
        seq_t cur_x, cur_y;
        compute_pair(cache_size + 1, &cur_x, &cur_y);

        /* Search existing pair. */
        size_t idx = 0;
        bool found = false;
        for (size_t i = 0; i < pair_cnt; ++i) {
            if (pairs[i].x == cur_x && pairs[i].y == cur_y) {
                idx = i;
                found = true;
                break;
            }
        }

        seq_t value;
        if (found) {
            value = (seq_t)(idx + 1);      /* 1‑based rank of first occurrence */
        } else {
            grow_pair_array();
            pairs[pair_cnt].x = cur_x;
            pairs[pair_cnt].y = cur_y;
            value = (seq_t)(pair_cnt + 1);
            ++pair_cnt;
        }

        grow_cache_array();
        cache_vals[cache_size++] = value;
    }

    return cache_vals[n - 1];
}
