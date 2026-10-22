/*
 * A335382  a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) - sigma(n) if
 *          nonnegative and not already in the sequence, otherwise a(n) =
 *          a(n-1) + sigma(n), where sigma(n) is the sum of the divisors of n.
 * https://oeis.org/A335382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence using the sum of divisors. a(0) = 0, a(1) = 1 For n >
 *            1: let s = sigma(n) if a(n‑1) ≥ s and (a(n‑1) − s) has not
 *            appeared before a(n) = a(n‑1) − s else a(n) = a(n‑1) + s
 * Range:     u8 n <= 59, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/sigma_of.h"

/* ---------- Seen values storage ------------------------------------- */

static seq_t *seen_tbl = NULL;
static size_t seen_used = 0;
static size_t seen_cap  = 0;

static void seen_expand(void)
{
    size_t new_cap = seen_cap ? seen_cap << 1 : 8;
    seq_t *new_arr = (seq_t *)realloc(seen_tbl, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    seen_tbl = new_arr;
    seen_cap = new_cap;
}

static bool seen_contains(seq_t key)
{
    if (!seen_tbl) return false;
    for (size_t i = 0; i < seen_used; ++i)
        if (seen_tbl[i] == key) return true;
    return false;
}

static void seen_insert(seq_t key)
{
    if (seen_contains(key)) return;
    if (seen_used == seen_cap) seen_expand();
    seen_tbl[seen_used++] = key;
}

/* ---------- Sequence cache ------------------------------------------ */

static seq_t *seq_cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

static void cache_expand(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    seq_cache = new_arr;
    cache_cap = new_cap;
}

static void init_sequence(void)
{
    if (cache_len) return;            /* already initialized */

    cache_expand();
    seq_cache[0] = (seq_t)0;
    seq_cache[1] = (seq_t)1;
    cache_len = 2;
    seen_insert(seq_cache[0]);
    seen_insert(seq_cache[1]);
}

/* ---------- Main function ------------------------------------------- */

seq_t a(idx_t n)
{
    init_sequence();

    if (n < cache_len) return seq_cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = seq_cache[i - 1];
        seq_t sig  = sigma_of(i);

        seq_t val;
        if (prev >= sig) {
            seq_t cand = prev - sig;
            if (!seen_contains(cand))
                val = cand;
            else
                val = prev + sig;
        } else {
            val = prev + sig;
        }

        if (cache_len == cache_cap) cache_expand();
        seq_cache[cache_len++] = val;
        seen_insert(val);
    }

    return seq_cache[n];
}
