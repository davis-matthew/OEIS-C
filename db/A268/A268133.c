/*
 * A268133  If a(n) is not a square, then a(n+1) = a(n) + a(n-1), else a(n+1)
 *          is the smallest positive integer not occurring earlier.
 * https://oeis.org/A268133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 55, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A268133
 *
 *  Recurrence:
 *     a(1) = 1, a(2) = 2
 *     For n ≥ 2:
 *        if a(n) is a perfect square then
 *              a(n+1) = smallest positive integer not yet occurring in the sequence
 *        else
 *              a(n+1) = a(n) + a(n‑1)
 *
 *  The function below computes a(n) for any n ≥ 1, memoising the sequence
 *  across calls so that an arbitrary order of queries is handled efficiently.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* ----------  static data  ---------- */
static seq_t *cache = NULL;        /* sequence cache, 0‑based for a(1)   */
static idx_t  cache_len = 0;       /* number of terms stored           */
static idx_t  cache_cap = 0;       /* allocated size of cache          */

static bool *seen = NULL;          /* seen[x] = true if x has appeared*/
static idx_t  seen_cap = 0;        /* current size of seen array – 1   */
static idx_t  next_missing = 1;    /* smallest positive integer not yet seen */

/* ----------  helpers  ---------- */
static void ensure_seen_cap(idx_t idx)
{
    if (idx <= seen_cap) return;
    idx_t new_cap = seen_cap ? seen_cap : 32;
    while (idx > new_cap) new_cap <<= 1;
    bool *new_seen = (bool *)realloc(seen, (new_cap + 1) * sizeof(bool));
    if (!new_seen) abort();
    seen = new_seen;
    for (idx_t i = seen_cap + 1; i <= new_cap; ++i) seen[i] = false;
    seen_cap = new_cap;
}

static void ensure_cache_cap(idx_t required)
{
    if (required <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (required > new_cap) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* ----------  sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) abort();                 /* sequence is 1‑based */

    /* --- first call: initialise global tables --- */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 2;                   /* we already know a(1) and a(2) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;

        seen_cap = 32;
        seen = (bool *)malloc((seen_cap + 1) * sizeof(bool));
        if (!seen) abort();
        for (idx_t i = 0; i <= seen_cap; ++i) seen[i] = false;
        seen[1]   = true;
        seen[2]   = true;
        next_missing = 3;                /* smallest unused positive integer */
    }

    /* --- compute terms up to n --- */
    while (cache_len < n) {
        seq_t a_prev  = cache[cache_len - 1];   /* a(n)    */
        seq_t a_prev2 = cache[cache_len - 2];   /* a(n‑1)  */
        seq_t a_next;

        seq_t sqrt_val = floor_sqrt(a_prev);
        if (sqrt_val * sqrt_val == a_prev) {     /* a(n) is a square */
            /* use the smallest missing positive integer   */
            idx_t missing = next_missing;
            ensure_seen_cap(missing);
            seen[missing] = true;
            a_next = missing;

            /* advance next_missing to the next unseen integer */
            ++missing;
            while (true) {
                ensure_seen_cap(missing);
                if (!seen[missing]) break;
                ++missing;
            }
            next_missing = missing;
        } else {                                 /* Fibonacci step  */
            a_next = a_prev + a_prev2;
        }

        /* store the new term */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = a_next;
        ++cache_len;

        /* mark the value as seen if it fits in the seen table */
        if (a_next <= seen_cap) {
            seen[(idx_t)a_next] = true;
            /* keep next_missing up to date */
            while (true) {
                ensure_seen_cap(next_missing);
                if (!seen[next_missing]) break;
                ++next_missing;
            }
        }
    }

    return cache[n - 1];
}
