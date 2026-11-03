/*
 * A351889  Table T(n,k) read by downward antidiagonals: period of n-step
 *          Fibonacci numbers mod k, n >= 1, k >= 1.
 * https://oeis.org/A351889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 73, u32 n <= 183, u64 n <= 226,
 *            u128 n <= 226, big any n
 * Validated: standard, pass, 90 terms through n = 218 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351889
 *  a(n) = the n-th entry of the table T(i,j) of periods of i-step Fibonacci
 *  numbers modulo j, read by downward antidiagonals.
 *
 *  This implementation builds the table on the fly up to the size needed
 *  for the queried index n and caches the computed periods.
 *
 *  All arithmetic uses the generic integer type seq_t.  Intermediate
 *  calculations are done with uint64_t because the moduli and orders are
 *  always small ( ≤ ~150 for n ≤ 10 000 ).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Compute the period of the n‑step Fibonacci sequence modulo k.
 * Uses initial terms 0,…,0,1.  Mod > 0.
 */
static uint64_t period_of_order_mod(unsigned int order, unsigned int mod)
{
    if (mod == 1) return 1;   /* all zero modulo 1 => period 1 */

    uint64_t *state = (uint64_t*)calloc(order, sizeof(uint64_t));
    uint64_t *init  = (uint64_t*)calloc(order, sizeof(uint64_t));

    state[order-1] = 1 % mod;        /* initial vector 0,…,0,1 */
    memcpy(init, state, order * sizeof(uint64_t));

    uint64_t sum = 0;
    for (size_t i = 0; i < order; ++i) sum += state[i];

    uint64_t period = 0;
    while (1) {
        uint64_t next = sum % mod;
        sum -= state[0];
        for (size_t i = 0; i + 1 < order; ++i) state[i] = state[i+1];
        state[order-1] = next;
        sum += next;
        ++period;

        /* check repetition of initial state */
        size_t i;
        for (i = 0; i < order; ++i)
            if (state[i] != init[i]) break;
        if (i == order) break;
    }

    free(state);
    free(init);
    return period;
}

/* Ensure range cast into seq_t (works for integer types and
 * arbitrary-precision integer classes).
 */
static inline seq_t to_seq(uint64_t v) { return (seq_t)v; }

/* -------------------  Period cache -------------------- */
static seq_t **period_cache = NULL;
static size_t period_capacity = 0;

/* Grow the cache to hold all entries i,j ≤ needed (1‑based). */
static void period_cache_ensure(size_t needed)
{
    if (needed <= period_capacity) return;

    /* allocate new rows */
    seq_t **new_rows = (seq_t**)calloc(needed + 1, sizeof(seq_t*));
    for (size_t i = 1; i <= needed; ++i)
        new_rows[i] = mem_new(seq_t, needed + 1);

    /* copy old data */
    if (period_capacity > 0) {
        for (size_t i = 1; i <= period_capacity; ++i) {
            for (size_t j = 1; j <= period_capacity; ++j)
                new_rows[i][j] = period_cache[i][j];
            mem_free(period_cache[i]);
        }
        free(period_cache);
    }

    period_cache   = new_rows;
    period_capacity = needed;
}

/* Return T(i,j) with caching.  1‑based indices. */
static seq_t get_T(uint64_t i, uint64_t j)
{
    if (i == 1 || j == 1) return to_seq(1);

    size_t need = (size_t)( i > j ? i : j );
    period_cache_ensure(need);

    seq_t val = period_cache[i][j];
    if (val != 0) return val;

    uint64_t per = period_of_order_mod((unsigned int)i, (unsigned int)j);
    period_cache[i][j] = to_seq(per);
    return period_cache[i][j];
}

/* -------------------  Main function -------------------- */
seq_t a(idx_t n)
{
    /* find diagonal d such that (d+1)(d+2)/2 ≥ n */
    idx_t d = 0;
    while ( (d + 1) * (d + 2) / 2 < n ) d++;

    /* number of terms before this diagonal */
    idx_t terms_before = d * (d + 1) / 2;

    /* zero‑based position inside the diagonal */
    idx_t pos0 = n - terms_before - 1;

    idx_t i = pos0 + 1;          /* row index (n‑step)   */
    idx_t j = d + 2 - i;         /* column index (modulus) */

    return get_T(i, j);
}
