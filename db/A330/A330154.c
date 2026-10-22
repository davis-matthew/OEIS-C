/*
 * A330154  Each term is visited according to the rule: move left (resp.
 *          right) a(n) places if a(n) is prime (resp. not prime), starting at
 *          n = 1: lexicographically earliest such sequence of distinct
 *          positive integers.
 * https://oeis.org/A330154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330154 – lexicographically earliest permutation of
   the positive integers such that for each index n
   the step n → n ± a(n) is to the left if a(n) is prime,
   to the right otherwise, and every index > 1 has exactly
   one predecessor. The sequence is generated greedily:
   at each index n choose the smallest unused positive integer k
   for which the target t = n − k (prime) or n + k (composite)
   satisfies t ≥ 2 and has no predecessor yet. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Static tables with dynamic growth. */
static seq_t *values = NULL;          /* a(n) values, 1‑based */
static size_t values_cap = 0;
static bool *used = NULL;             /* used[k] indicates k has been assigned */
static size_t used_cap = 0;
static bool *has_pred = NULL;         /* has_pred[m] indicates m already has a predecessor */
static size_t pred_cap = 0;
static idx_t computed = 0;            /* largest n that has been fully processed */

/* Helper to enlarge the values array. */
static void ensure_values(size_t n)
{
    if (n < values_cap) return;
    size_t newcap = values_cap ? values_cap : 16;
    while (newcap <= n) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(values, newcap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = values_cap; i < newcap; ++i) tmp[i] = 0;
    values = tmp;
    values_cap = newcap;
}

/* Helper to enlarge the used array. */
static void ensure_used(size_t k)
{
    if (k < used_cap) return;
    size_t newcap = used_cap ? used_cap : 16;
    while (newcap <= k) newcap <<= 1;
    bool *tmp = (bool*)realloc(used, newcap * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = used_cap; i < newcap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = newcap;
}

/* Helper to enlarge the has_pred array. */
static void ensure_pred(size_t m)
{
    if (m < pred_cap) return;
    size_t newcap = pred_cap ? pred_cap : 16;
    while (newcap <= m) newcap <<= 1;
    bool *tmp = (bool*)realloc(has_pred, newcap * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = pred_cap; i < newcap; ++i) tmp[i] = false;
    has_pred = tmp;
    pred_cap = newcap;
}

/* Compute a(n) for all indices up to n, filling the static tables. */
static void compute_up_to(idx_t n)
{
    if (n <= computed) return;

    /* Allocate minimal working space. */
    ensure_values((size_t)n);
    ensure_used(1);          /* we will access used[1] for a(1) */
    ensure_pred(2);          /* a(1) points to index 2 */

    /* Initialise root if this is the first call. */
    if (computed == 0) {
        values[1] = (seq_t)1;
        used[1] = true;           /* value 1 is used */
        has_pred[2] = true;       /* index 2 has predecessor 1 */
        computed = 1;
    }

    /* Extend computation. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Choose smallest unused k satisfying the rules. */
        for (seq_t k = 1; ; ++k) {
            /* Grow used[] if necessary. */
            if ((size_t)k >= used_cap) ensure_used((size_t)k);
            if (used[(size_t)k]) continue;   /* already used */

            bool prime = is_prime((idx_t)k);
            idx_t target;

            if (prime) {
                /* Prime step goes left: ensure the target is ≥ 2. */
                if ((idx_t)k > i - 2) continue;   /* would go to 1 or ≤ 0 */
                target = i - (idx_t)k;
            } else {
                /* Composite step goes right. */
                target = i + (idx_t)k;
            }

            if (target < 2) continue;           /* safety, though handled above */

            if ((size_t)target >= pred_cap) ensure_pred((size_t)target);
            if (has_pred[(size_t)target]) continue;   /* target already has a predecessor */

            /* Accept this k. */
            values[(size_t)i] = k;
            used[(size_t)k] = true;
            has_pred[(size_t)target] = true;
            break;
        }
    }
    computed = n;
}

/* Public entry function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for non‑positive indices */
    compute_up_to(n);
    return values[(size_t)n];
}
