/*
 * A327907  Numbers with more than one factorization into at factors > 1 with
 *          integer mean.
 * https://oeis.org/A327907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A327907 – Numbers with more than one factorization into
   factors > 1 with integer mean.  This function generates the
   nth term (offset 1) by searching integers in increasing order
   until the required number of qualifying terms is found.  It uses
   a depth‑first search to enumerate factorisations and stops
   as soon as two valid factorizations are found for a candidate. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t x) */

static seq_t *seq_cache = NULL;
static size_t   seq_len  = 0;
static size_t   seq_cap  = 0;
static idx_t   next_candidate = 2;   /* next integer to test */

/* Grow the sequence cache to at least 'needed' entries. */
static void ensure_cache_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Count factorisations of 'rem' using factors >= 'min_f'.
   'sum' and 'len' hold the cumulative sum and number of factors
   chosen so far.  Updates *cnt whenever a factorisation having
   an integer mean is found.  The search stops immediately when
   *cnt reaches 2. */
static void dfs_factorisations(seq_t rem,
                               seq_t min_f,
                               seq_t sum,
                               seq_t len,
                               int *cnt)
{
    if (*cnt >= 2) return;

    seq_t lim = floor_sqrt(rem);
    bool found = false;
    for (seq_t f = min_f; f <= lim; ++f) {
        if (rem % f == 0) {
            found = true;
            seq_t sub = rem / f;
            if (sub >= f) {
                /* Count the 2‑factor factorisation (f, sub). */
                seq_t sum1 = sum + f + sub;
                seq_t len1 = len + 2;
                if (len1 > 0 && (sum1 % len1) == 0) {
                    ++(*cnt);
                    if (*cnt >= 2) return;
                }
                /* Recurse to extend the factorisation. */
                dfs_factorisations(sub, f, sum + f, len + 1, cnt);
                if (*cnt >= 2) return;
            }
        }
    }

    /* If no further divisor was found, consider the situation
       where the current remainder 'rem' is taken as the last
       factor (this occurs only when we are not at the root
       call). */
    if (!found && len > 0) {
        seq_t sum2 = sum + rem;
        seq_t len2 = len + 1;
        if ((sum2 % len2) == 0) {
            ++(*cnt);
            if (*cnt >= 2) return;
        }
    }
}

/* Determine whether an integer has at least two distinct
   factorisations into factors > 1 whose arithmetic mean is
   an integer. */
static bool has_at_least_two_factorisations(seq_t n)
{
    int cnt = 0;

    /* The trivial factorisation consisting of the single factor n. */
    ++cnt;               /* always an integer mean (mean = n) */
    if (cnt >= 2) return true;

    dfs_factorisations(n, 2, 0, 0, &cnt);
    return cnt >= 2;
}

/* Main sequence function.  Returns the nth term (offset 1). */
seq_t a(idx_t n)
{
    assert(n >= 1);
    /* Make sure that seq_cache has at least n entries. */
    while (seq_len < n) {
        if (has_at_least_two_factorisations((seq_t)next_candidate)) {
            ensure_cache_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
