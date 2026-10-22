/*
 * A316364  Number of factorizations of n into factors > 1 such that every
 *          distinct submultiset of the factors has a different average.
 * https://oeis.org/A316364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8329, u16 n <= 8329, u32 n <= 8329, u64 n <= 8329,
 *            u128 n <= 8329, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A316364: number of factorizations of n into distinct factors >1
   such that every distinct submultiset of the factors has a different average. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t num;
    seq_t den;
} Fraction;

/* Static cache for memoisation */
static seq_t *cache = NULL;
static bool  *computed = NULL;
static idx_t cache_cap = 0;

/* Recursive enumeration of distinct factorizations */
static void dfs(idx_t rem, idx_t minf, idx_t *arr, idx_t depth, idx_t *cnt)
{
    /* Candidate where rem itself is the last factor */
    if (rem >= minf) {
        arr[depth] = rem;
        /* Validate the constructed factor set */
        idx_t k = depth + 1;          /* number of factors */
        /* Generate all non‑empty subsets and check average uniqueness */
        idx_t subsets = ((idx_t)1) << k;
        /* Subset count will be small (k <= 6 for n <= 10000) */
        Fraction fracs[128];
        idx_t fcnt = 0;
        bool ok = true;
        for (idx_t mask = 1; mask < subsets; ++mask) {
            seq_t sum = 0;
            idx_t sz  = 0;
            for (idx_t i = 0; i < k; ++i)
                if (mask & ((idx_t)1 << i)) {
                    sum += (seq_t)arr[i];
                    sz++;
                }
            seq_t g = gcd(sum, (seq_t)sz);
            seq_t num = sum / g;
            seq_t den = (seq_t)sz / g;
            /* Check against previously seen averages */
            for (idx_t j = 0; j < fcnt; ++j) {
                if (fracs[j].num == num && fracs[j].den == den) {
                    ok = false;
                    break;
                }
            }
            if (!ok) break;
            fracs[fcnt].num = num;
            fracs[fcnt].den = den;
            ++fcnt;
        }
        if (ok) (*cnt)++;
    }

    /* Recurse with each divisor f >= minf and f <= sqrt(rem) */
    for (idx_t f = minf; f <= rem / f; ++f) {
        if (rem % f == 0) {
            arr[depth] = f;
            dfs(rem / f, f + 1, arr, depth + 1, cnt);
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Resize cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        cache     = (seq_t *)realloc(cache,     sizeof(seq_t)     * new_cap);
        computed  = (bool  *)realloc(computed,  sizeof(bool)      * new_cap);
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            cache[i]    = 0;
            computed[i] = false;
        }
        cache_cap = new_cap;
    }
    if (computed[n]) return cache[n];

    seq_t result;
    if (n == 1) {
        result = 1;  /* by definition */
    } else {
        static idx_t factors[32];
        idx_t count = 0;
        dfs(n, 2, factors, 0, &count);
        result = (seq_t)count;
    }

    cache[n]    = result;
    computed[n] = true;
    return result;
}
