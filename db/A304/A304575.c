/*
 * A304575  a(n) = Sum_{d|n} #{k < d, k squarefree and relatively prime to d}.
 * https://oeis.org/A304575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} #{ k ≤ d : k is squarefree and gcd(k,d)=1 }
 *
 * The implementation follows the definition directly.
 * For each divisor d of n the function count_coprime_squarefree
 * counts the qualifying k by iterating from 1 to d.
 * A small cache of square‑free flags is maintained up to the largest
 * divisor requested so far.
 *
 * Dependencies: "../utils/gcd.h"
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* square‑free flag cache                                              */
/* ------------------------------------------------------------------ */
static bool *squarefree_cache = NULL;
static idx_t squarefree_cache_size = 0;   /* number of valid entries */

/* ensure the cache covers values up to nmax (inclusive) */
static void ensure_squarefree_cache(idx_t nmax)
{
    if (squarefree_cache_size >= nmax + 1)
        return;

    bool *new_cache = (bool *)malloc((size_t)(nmax + 1) * sizeof(bool));
    if (!new_cache) return;  /* allocation failure ignored: behaviour undefined */

    /* initialise all to true */
    for (idx_t i = 0; i <= nmax; ++i)
        new_cache[i] = true;

    /* sieve of squares: multiples of p^2 are not square‑free */
    for (idx_t p = 2; p * p <= nmax; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= nmax; j += sq)
            new_cache[j] = false;
    }

    free(squarefree_cache);
    squarefree_cache = new_cache;
    squarefree_cache_size = nmax + 1;
}

/* ------------------------------------------------------------------ */
/* count of k ≤ d that are square‑free and coprime with d              */
/* ------------------------------------------------------------------ */
static seq_t count_coprime_squarefree(idx_t d)
{
    if (d == 0)
        return 0;

    ensure_squarefree_cache(d);

    seq_t cnt = 0;
    seq_t d_seq = (seq_t)d;

    for (idx_t k = 1; k <= d; ++k) {
        if (squarefree_cache[k] && gcd((seq_t)k, d_seq) == 1)
            ++cnt;
    }

    return cnt;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, defensive */

    seq_t result = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            result += count_coprime_squarefree(i);

            idx_t d2 = n / i;
            if (d2 != i)
                result += count_coprime_squarefree(d2);
        }
    }

    return result;
}
