/*
 * A266112  Characteristic function for A266114 (numbers that are least
 *          siblings in A263267-tree).
 * https://oeis.org/A266112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there is no k < n with k - tau(k) = n - tau(n); 0
 *            otherwise. Here tau(m) = A000005(m), the number of divisors
 * Range:     u8 n <= 9668, u16 n <= 9668, u32 n <= 9668, u64 n <= 9668,
 *            u128 n <= 9668, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/divisor_count_u64.h"

static idx_t *min_idx = NULL;   /* storing the smallest index seen for each d = n - tau(n) */
static idx_t  max_n   = 0;      /* largest n that has been processed */
static size_t min_cap = 0;      /* allocated capacity for min_idx */

/* Grow the min_idx array to at least 'needed' entries. */
static void grow_min_idx(size_t needed)
{
    if (needed <= min_cap)
        return;
    size_t new_cap = needed;
    /* grow by a factor to keep reallocs rare */
    while (new_cap < needed)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(min_idx, new_cap * sizeof(idx_t));
    /* assume out‑of‑memory is fatal; the calling environment will detect it */
    if (!tmp)
        abort();
    /* initialise new part to 0 (not seen) */
    memset(tmp + min_cap, 0, (new_cap - min_cap) * sizeof(idx_t));
    min_idx = tmp;
    min_cap = new_cap;
}

/* Ensure that all values up to 'n' have been processed. */
static void preprocess_up_to(idx_t n)
{
    if (n <= max_n)
        return;
    for (idx_t i = max_n + 1; i <= n; ++i) {
        uint64_t tau = divisor_count_u64((uint64_t)i);
        idx_t d = i - (idx_t)tau;
        grow_min_idx((size_t)d + 1);
        if (min_idx[d] == 0)
            min_idx[d] = i;   /* first occurrence, remember minimal index */
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* out‑of‑range but return 0 safely */
        return (seq_t)0;

    preprocess_up_to(n);

    /* determine the value for n itself */
    uint64_t tau_n = divisor_count_u64((uint64_t)n);
    idx_t d_n = n - (idx_t)tau_n;

    /* if the minimal index for this d is this n, it is the first occurrence */
    return (seq_t)(min_idx[d_n] == n ? 1 : 0);
}
