/*
 * A319786  Number of factorizations of n where no two factors are relatively
 *          prime.
 * https://oeis.org/A319786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4058, u16 n <= 9583, u32 n <= 9583, u64 n <= 9583,
 *            u128 n <= 9583, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A319786: Number of unordered factorizations of n in which every pair of
   factors has gcd > 1.  The function enumerates all such factorizations
   recursively, using a simple back‑tracking search. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

#define MAX_CACHE 10000  /* maximum n for which we keep a memo table */

/* memoisation storage */
static seq_t  cache[MAX_CACHE + 1];
static bool   computed[MAX_CACHE + 1];

/* ---------------------------------------------------------------------- */
/* Recursive search for factorizations of 'rem' with factors >= 'last'    */
/* ---------------------------------------------------------------------- */
static void dfs(idx_t rem, idx_t last, idx_t *path, idx_t depth, seq_t *cnt)
{
    for (idx_t d = last; d * d <= rem; ++d)
    {
        if (rem % d != 0)
            continue;                     /* skip non‑divisors */

        /* check gcd with all previously chosen factors */
        bool ok = true;
        for (idx_t i = 0; i < depth; ++i)
        {
            if (gcd((seq_t)d, (seq_t)path[i]) == 1)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        /* recurse with the new divisor */
        path[depth] = d;
        dfs(rem / d, d, path, depth + 1, cnt);
    }

    /* treat the remaining number as the last factor */
    if (rem >= last)
    {
        bool ok = true;
        for (idx_t i = 0; i < depth; ++i)
        {
            if (gcd((seq_t)rem, (seq_t)path[i]) == 1)
            {
                ok = false;
                break;
            }
        }
        if (ok)
            (*cnt)++;          /* a valid factorization found */
    }
}

/* ---------------------------------------------------------------------- */
/* Compute a(n) when it has not been cached yet.                        */
/* ---------------------------------------------------------------------- */
static seq_t compute_value(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t   cnt = 0;
    idx_t   path[32];          /* enough for n <= 10000 */
    dfs(n, 2, path, 0, &cnt);
    return cnt;
}

/* ---------------------------------------------------------------------- */
/* Public entry point                                                     */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* not used by OEIS offset 1 */

    if (n <= MAX_CACHE)
    {
        if (!computed[n])
        {
            cache[n] = compute_value(n);
            computed[n] = true;
        }
        return cache[n];
    }

    /* for n > MAX_CACHE we compute on the fly (no caching) */
    return compute_value(n);
}
