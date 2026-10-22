/*
 * A333487  Number of inseparable factorizations of n into factors > 1.
 * https://oeis.org/A333487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A333487: 
   Number of inseparable factorizations of n into factors > 1.
   A factorization is inseparable if its multiset of factors cannot be
   arranged so that no equal factors are adjacent, i.e. if the
   multiplicity of some value exceeds (k+1)/2, where k is the number of
   factors.  We enumerate all (order‑independent) multiplicative
   partitions of n and count those satisfying the condition.  A cache
   memoizes results to allow arbitrary call order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* cache for values already computed */
static seq_t *cache = NULL;
static bool  *cache_flag = NULL;
static size_t cache_cap = 0;

/* ensure cache capacity for indices up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;                    /* already enough */

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();       /* out of memory */
    bool *new_flag  = (bool *)realloc(cache_flag, new_cap * sizeof(bool));
    if (!new_flag) abort();        /* out of memory */

    /* initialise new entries */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        new_flag[i] = false;
    }

    cache = new_cache;
    cache_flag = new_flag;
    cache_cap = new_cap;
}

/* recursive enumeration of factorisations of rem.
   Factors are chosen in nondecreasing order.
   factors[0..depth-1] holds the current factor list.
   count accumulates the number of inseparable factorizations. */
static void dfs(idx_t rem, idx_t min_fac, idx_t *factors,
                int depth, seq_t *count)
{
    if (rem == 1) {
        if (depth < 2)     /* need at least two factors */
            return;

        /* compute maximal multiplicity of any factor in the list */
        int maxm = 1, cur = 1;
        for (int i = 1; i < depth; ++i) {
            if (factors[i] == factors[i - 1]) {
                ++cur;
            } else {
                if (cur > maxm) maxm = cur;
                cur = 1;
            }
        }
        if (cur > maxm) maxm = cur;

        /* inseparable if max multiplicity > (k+1)/2 */
        int k = depth;
        if (maxm > (k + 1) / 2)
            ++(*count);
        return;
    }

    /* try all divisors >= min_fac up to sqrt(rem) */
    for (idx_t f = min_fac; f * f <= rem; ++f) {
        if (rem % f == 0) {
            factors[depth] = f;
            dfs(rem / f, f, factors, depth + 1, count);
        }
    }

    /* also consider rem itself as a factor, if allowed */
    if (rem >= min_fac) {
        factors[depth] = rem;
        dfs(1, rem, factors, depth + 1, count);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)          /* sequence is defined for n >= 1 */
        return 0;

    ensure_cache(n);

    if (cache_flag[n])          /* already computed */
        return cache[n];

    /* compute a(n) */
    seq_t cnt = 0;
    idx_t factors[64];          /* enough for n <= 10000 */
    dfs(n, 2, factors, 0, &cnt);

    cache[n] = cnt;
    cache_flag[n] = true;
    return cnt;
}
