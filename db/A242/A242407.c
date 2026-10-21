/*
 * A242407  Numbers such that in ternary representation all pairs of adjacent
 *          digits have sums not greater than 2.
 * https://oeis.org/A242407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th non‑negative integer whose base‑3 digits *
 *            satisfy that every adjacent pair of digits has sum ≤ 2. *
 *            Computation: incrementally search integers, test the adjacency
 *            rule, * cache the results, and return the n‑th entry
 * Range:     u8 n <= 80, u16 n <= 4797, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define CACHE_CAP 20000

static seq_t cache[CACHE_CAP];
static idx_t cache_len = 0;     /* number of stored terms              */
static idx_t next_candidate = 0;/* next integer to test                 */

/* Return true if all adjacent base‑3 digits of x have sum ≤ 2. */
static bool valid(seq_t x)
{
    if (x == 0)
        return true;
    seq_t prev = x % 3;
    x /= 3;
    while (x)
    {
        seq_t d = x % 3;
        if (prev + d > 2)
            return false;
        prev = d;
        x /= 3;
    }
    return true;
}

/* Ensure we have at least n terms stored (1‑based). */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        for (;;)
        {
            seq_t val = (seq_t)next_candidate;
            if (valid(val))
            {
                cache[cache_len++] = val;
                ++next_candidate;      /* move to next integer */
                break;                  /* exit inner loop      */
            }
            ++next_candidate;
        }
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of range – behaviour irrelevant               */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
