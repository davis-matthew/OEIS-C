/*
 * A256660  Number of terms in the minimal alternating Fibonacci
 *          representation of n.
 * https://oeis.org/A256660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal number of terms in an alternating Fibonacci
 *            representation of n
 * Range:     u8 n <= 188, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_FIB 200

static seq_t fib[MAX_FIB];
static seq_t pref[MAX_FIB];
static size_t fib_size = 0;
static idx_t fib_cache_limit = 0;

/* global target value for the current search */
static seq_t target_global = 0;

/* Build Fibonacci numbers up to 2*n (or until overflow) and their prefix sums. */
static void update_fib_cache(idx_t n) {
    if (n <= fib_cache_limit) return;
    seq_t needed = (seq_t)n * ((seq_t)2u);   /* may overflow, but fitting seq_t is assumed */
    if (fib_size == 0) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        pref[0] = (seq_t)0;
        pref[1] = fib[1];
        pref[2] = pref[1] + fib[2];
        fib_size = 2;
    }
    size_t i = fib_size;
    while (i + 1 < MAX_FIB) {
        seq_t next = fib[i] + fib[i-1];
        if (next < fib[i]) break;             /* overflow check */
        if (next > needed) break;
        ++i;
        fib[i] = next;
        pref[i] = pref[i-1] + next;
    }
    fib_size = i;
    fib_cache_limit = n;
}

/* Depth‑first search for a representation with at most max_depth terms. */
static bool dfs_rec(size_t idx, size_t depth, size_t max_depth, seq_t current) {
    if (depth == max_depth)            return current == target_global;
    if (idx == 0)                      return current == target_global;

    /* pruning using remaining sum bounds */
    seq_t remaining = pref[idx];
    seq_t upper = current + remaining;
    if (target_global > upper) return false;
    if (current >= remaining) {
        seq_t lower = current - remaining;
        if (target_global < lower) return false;
    }

    /* option 1: skip this index */
    if (dfs_rec(idx-1, depth, max_depth, current)) return true;

    /* option 2: include this index with the appropriate sign */
    seq_t new_sum = (depth & 1u) ? (current - fib[idx]) : (current + fib[idx]);
    return dfs_rec(idx-1, depth+1, max_depth, new_sum);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    target_global = (seq_t)n;
    update_fib_cache(n);
    size_t lastIdx = fib_size;          /* highest Fibonacci index used */

    for (size_t depth = 1; depth <= 20; ++depth) {   /* 20 is safely above the known maximum */
        if (dfs_rec(lastIdx, 0, depth, (seq_t)0)) {
            return (seq_t)depth;
        }
    }

    /* Should not happen for valid inputs.  Return a sentinel value. */
    return (seq_t)-1;
}
