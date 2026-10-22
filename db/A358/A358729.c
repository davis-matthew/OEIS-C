/*
 * A358729  Difference between the number of nodes and the node-height of the
 *          rooted tree with Matula-Goebel number n.
 * https://oeis.org/A358729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (#nodes(n) – height(n)) for the rooted tree with
 *            Matula-Goebel number n
 * Range:     u8 n <= 9308, u16 n <= 9308, u32 n <= 9308, u64 n <= 9308,
 *            u128 n <= 9308, big any n
 * Helpers:   alloc_seq_array, build_primes_upto, factor_u64, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *nodes      = NULL;
static seq_t *heights    = NULL;
static size_t node_cap   = 0;
static bool  *computed   = NULL;
static size_t comp_cap   = 0;

static uint64_t *prime_arr    = NULL;
static size_t    prime_cnt    = 0;
static size_t    prime_limit  = 0;
static size_t   *prime_index  = NULL;

static void ensure_node_cache(idx_t n)
{
    if (node_cap > n) return;
    size_t new_cap = (node_cap == 0) ? (n + 1) : node_cap * 2;
    if (new_cap < n + 1) new_cap = n + 1;

    seq_t *new_nodes   = (seq_t *)alloc_seq_array(new_cap);
    seq_t *new_heights = (seq_t *)alloc_seq_array(new_cap);

    if (nodes) {
        size_t i;
        for (i = 0; i < node_cap; ++i) {
            new_nodes[i]   = nodes[i];
            new_heights[i] = heights[i];
        }
        free_seq_array(nodes);
        free_seq_array(heights);
    }
    nodes   = new_nodes;
    heights = new_heights;
    node_cap = new_cap;
}

static void ensure_computed_cache(idx_t n)
{
    if (comp_cap > n) return;
    size_t new_cap = (comp_cap == 0) ? (n + 1) : comp_cap * 2;
    if (new_cap < n + 1) new_cap = n + 1;

    bool *new_computed = (bool *)calloc(new_cap, sizeof(bool));
    if (computed) {
        memcpy(new_computed, computed, comp_cap * sizeof(bool));
        free(computed);
    }
    computed = new_computed;
    comp_cap = new_cap;
}

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_arr) {
        free(prime_arr);
        free(prime_index);
    }
    build_primes_upto(limit, &prime_arr, &prime_cnt);
    prime_limit = limit;

    prime_index = (size_t *)calloc(prime_limit + 1, sizeof(size_t));
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_arr[i] <= prime_limit)
            prime_index[prime_arr[i]] = i + 1; /* primes are 2-based indexed */
    }
}

static void compute_rec(idx_t n)
{
    ensure_node_cache(n);
    ensure_computed_cache(n);
    if (computed[n]) return;

    if (n == 1) {
        nodes[1]   = (seq_t)1;
        heights[1] = (seq_t)1;
        computed[1] = true;
        return;
    }

    ensure_primes_upto(n);

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t total_nodes = (seq_t)1;
    seq_t max_height  = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        idx_t branch = (idx_t)prime_index[p];   /* index of prime p */
        if (branch == 0) {
            /* Should not happen for prime factors produced by factor_u64. */
            continue;
        }
        ensure_node_cache(branch);
        ensure_computed_cache(branch);
        compute_rec(branch);

        total_nodes += (seq_t)e * nodes[branch];
        if (heights[branch] > max_height)
            max_height = heights[branch];
    }

    nodes[n]   = total_nodes;
    heights[n] = ((seq_t)1) + max_height;
    computed[n] = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, offset starts at 1 */
    ensure_node_cache(n);
    ensure_computed_cache(n);
    compute_rec(n);
    return nodes[n] - heights[n];
}
