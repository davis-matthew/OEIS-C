/*
 * A372043  a(1) = 1. For n > 1 a(n) is the smallest unused k such that
 *          A007947(k*a(n-1)) is novel.
 * https://oeis.org/A372043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least unused k such that rad(k*a(n-1)) is novel (OEIS
 *            A372043)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/rad_uint.h"

/* ---------- Static state for memoisation ---------- */
static seq_t *memo      = NULL;    /* a(1) … a(current_len) */
static size_t memo_len  = 0;       /* number of stored terms */

static bool  *used      = NULL;    /* used[i] == true if i already appears */
static size_t used_size = 0;       /* allocated size of 'used' array */

static uint64_t *rad_vals = NULL;  /* radicals of a(i)*a(i+1) for i=1..nn-1 */
static size_t rad_count   = 0;     /* number of radicals stored */

/* Ensure that 'used' can index up to 'sz-1' (i.e. size sz). */
static void ensure_used(size_t sz)
{
    if (sz <= used_size) return;
    bool *tmp = (bool *)realloc(used, sz * sizeof(bool));
    if (!tmp) exit(1);
    /* initialise the new portion to false */
    memset(tmp + used_size, 0, (sz - used_size) * sizeof(bool));
    used     = tmp;
    used_size = sz;
}

/* Linear search for a radical that has already appeared. */
static bool rad_exists(uint64_t r)
{
    for (size_t i = 0; i < rad_count; ++i)
        if (rad_vals[i] == r) return true;
    return false;
}

/* Build the sequence up to index 'n' (1‑based). */
static void compute_until(idx_t n)
{
    if (n <= memo_len) return;          /* already computed */

    /* First call: initialise a(1)=1. */
    if (!memo) {
        memo = (seq_t *)malloc(sizeof(seq_t));
        if (!memo) exit(1);
        memo[0] = (seq_t)1;
        memo_len = 1;
        /* used[1] == true (value 1 is now used) */
        ensure_used(2);                 /* indices 0 and 1 */
        used[1] = true;
    }

    for (idx_t cur = memo_len + 1; cur <= n; ++cur) {
        seq_t last = memo[cur - 2];     /* a(cur-1) */
        uint64_t candidate = 2;
        seq_t chosen = 0;
        uint64_t chosen_rad = 0;

        /* Find the least unused candidate with a novel radical. */
        for (;;) {
            if (candidate < used_size && used[candidate]) {
                ++candidate;
                continue;
            }
            /* product = last * candidate (both small enough for 64‑bit) */
            uint64_t prod = (uint64_t)last * candidate;
            uint64_t rad   = rad_uint(prod);
            if (!rad_exists(rad)) {
                chosen = (seq_t)candidate;
                chosen_rad = rad;
                break;
            }
            ++candidate;
        }

        /* Append the chosen term. */
        seq_t *tmp = (seq_t *)realloc(memo, (memo_len + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        memo = tmp;
        memo[memo_len++] = chosen;

        /* Mark the chosen number as used. */
        ensure_used(candidate + 1);       /* ensure we can index 'candidate' */
        used[candidate] = true;

        /* Store the radical of this new consecutive pair. */
        uint64_t *rtmp = (uint64_t *)realloc(rad_vals,
                                            (rad_count + 1) * sizeof(uint64_t));
        if (!rtmp) exit(1);
        rad_vals = rtmp;
        rad_vals[rad_count++] = chosen_rad;
    }
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    compute_until(n);
    return memo[n - 1];
}
