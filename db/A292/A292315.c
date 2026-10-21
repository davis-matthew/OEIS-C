/*
 * A292315  Positive integers not divisible by any number of the form 2^n + 1
 *          for n >= 0.
 * https://oeis.org/A292315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer that is not divisible by any integer
 *            of the form 2^k + 1 (k ≥ 0)
 * Range:     u8 n <= 32, u16 n <= 7301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of computed terms          */
static size_t cache_size = 0;        /* number of stored terms           */
static size_t cache_cap   = 0;        /* capacity of cache                */

static seq_t *fermat = NULL;        /* list of Fermat numbers 2^k+1, k≥1 */
static size_t fermat_size = 0;
static size_t fermat_cap   = 0;
static seq_t  last_f = 0;           /* last Fermat number computed      */

/* Ensure that all Fermat numbers ≤ limit are in 'fermat' array. */
static void ensure_fermat_upto(seq_t limit)
{
    /* Generate the first Fermat number (k = 1) if the list is empty. */
    if (fermat_size == 0) {
        last_f = 3; /* 2^1 + 1 */
        if (fermat_cap == 0) {
            fermat_cap = 16;
            fermat = (seq_t*)malloc(fermat_cap * sizeof(seq_t));
        }
        fermat[fermat_size++] = last_f;
    }

    /* While the next Fermat number would be ≤ limit, generate it. */
    while (limit >= last_f) {
        seq_t next_f = (last_f << 1) - 1; /* 2*last_f - 1  = 2^(k+1)+1 */
        /* Grow the array if needed */
        if (fermat_cap <= fermat_size) {
            fermat_cap *= 2;
            fermat = (seq_t*)realloc(fermat, fermat_cap * sizeof(seq_t));
        }
        fermat[fermat_size++] = last_f = next_f;
        if (next_f > limit) break;          /* guard against overflow cycles */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for non‑positive indices */

    /* Ensure the cache contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Grow cache if needed */
        if (cache_cap <= cache_size) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Candidate to test */
        static seq_t candidate = 1;
        seq_t m = candidate;

        bool good = true;

        /* Skip even numbers (2 is 2^0+1) */
        if ((m & 1) == 0) {
            good = false;
        } else {
            /* Ensure the Fermat numbers needed are available */
            ensure_fermat_upto(m);
            /* Test divisibility by all Fermat numbers ≤ m */
            for (size_t i = 0; i < fermat_size && fermat[i] <= m; ++i) {
                if ((m % fermat[i]) == 0) {
                    good = false;
                    break;
                }
            }
        }

        /* Store if valid */
        if (good) {
            cache[cache_size++] = m;
        }

        /* Advance to next integer */
        candidate = m + 1;
    }

    return cache[n - 1];
}
