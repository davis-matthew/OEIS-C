/*
 * A241783  Numbers that cannot be partitioned into distinct parts of the form
 *          3^k - 2^k, cf. A001047.
 * https://oeis.org/A241783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer that cannot be expressed as a sum of
 *            distinct numbers of the form 3^k - 2^k (k>=1)
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *missing = NULL;
static size_t miss_size = 0;
static size_t miss_cap = 0;

static idx_t processed_limit = 0;   /* highest value up to which reachability has been computed */
static idx_t scanned_up_to   = 0;   /* highest value up to which missing numbers have been collected */

/* Ensure that the missing array has room for one more element. */
static void ensure_miss_capacity(void)
{
    if (miss_size == miss_cap) {
        size_t new_cap = miss_cap ? miss_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(missing, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        missing = tmp;
        miss_cap = new_cap;
    }
}

/* Append a new missing value. */
static void add_missing(seq_t v)
{
    ensure_miss_capacity();
    missing[miss_size++] = v;
}

/* Extend reachability computations and collect missing numbers up to new_limit. */
static void extend_to_limit(idx_t new_limit)
{
    if (new_limit <= processed_limit) return;

    /* Generate coin values 3^k - 2^k that are <= new_limit. */
    seq_t coins[128];
    size_t coin_cnt = 0;
    seq_t pow3 = 1;
    seq_t pow2 = 1;
    for (idx_t k = 1; ; ++k) {
        seq_t prev_pow3 = pow3;
        seq_t prev_pow2 = pow2;
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
        /* detect unsigned overflow */
        if (pow3 < prev_pow3 || pow2 < prev_pow2) break;
        seq_t val = pow3 - pow2;
        if (val > (seq_t)new_limit) break;
        coins[coin_cnt++] = val;
        if (coin_cnt == sizeof(coins)/sizeof(coins[0])) break;
    }

    /* Allocate reachability array. */
    size_t reach_sz = (size_t)new_limit + 1;
    bool *reach = (bool *)calloc(reach_sz, sizeof(bool));
    if (!reach) exit(EXIT_FAILURE);
    reach[0] = true;

    /* Subset–sum DP with each coin used at most once. */
    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t coin = (idx_t)coins[ci];
        for (idx_t j = new_limit; j >= coin; --j) {
            if (reach[j - coin]) reach[j] = true;
        }
    }

    /* Record missing numbers. */
    for (idx_t i = scanned_up_to + 1; i <= new_limit; ++i) {
        if (!reach[i]) add_missing((seq_t)i);
    }

    /* Clean up. */
    free(reach);
    processed_limit = new_limit;
    scanned_up_to   = new_limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; return 0 for safety */
    /* Ensure that we have computed at least n missing numbers. */
    while (miss_size < (size_t)n) {
        idx_t new_lim = processed_limit > 0 ? processed_limit * 2 + 1000 : 1000;
        extend_to_limit(new_lim);
    }
    return missing[n - 1];
}
