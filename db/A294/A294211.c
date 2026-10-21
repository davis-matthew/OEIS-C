/*
 * A294211  Sum of the parts in the partitions of n into two squarefree parts
 *          or two nonsquarefree parts.
 * https://oeis.org/A294211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{i=1..floor(n/2)} [A008966(i) = A008966(n-i)]
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A008966(i) = 1 if i is squarefree, otherwise 0. */

#include <stdlib.h>
#include <stdbool.h>

static bool *squarefree = NULL;      /* squarefree status, 1 if true */
static idx_t squarefree_size = 0;    /* allocated size */

static seq_t *a_cache = NULL;        /* memoized a(n) values */
static idx_t a_cache_size = 0;

/* Ensure squarefree array covers at least up to n, using sieve of square multiples */
static void ensure_squarefree(idx_t n)
{
    if (n < squarefree_size)
        return;
    /* allocate or reallocate */
    idx_t new_size = n + 1;
    bool *new_arr = (bool*)realloc(squarefree, new_size * sizeof(bool));
    if (!new_arr)
        return; /* allocation failure, silently skip (unlikely in tests) */
    squarefree = new_arr;
    /* initialize new entries to true */
    for (idx_t i = squarefree_size; i <= n; ++i)
        squarefree[i] = true;
    /* sieve square factors */
    for (idx_t p = 2; p * p <= (idx_t)(n); ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= n; j += sq)
            squarefree[j] = false;
    }
    squarefree_size = new_size;
}

/* Ensure a_cache covers up to n */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *new_arr = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_arr)
        return;
    a_cache = new_arr;
    /* initialize new entries to 0 */
    for (idx_t i = a_cache_size; i <= n; ++i)
        a_cache[i] = 0;
    a_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but return 0 */

    ensure_a_cache(n);
    if (a_cache[n] != 0)    /* 0 may be a legitimate value for n=1,2? ; however
                              a(1)=0, so we need a separate visited flag.
                              Instead we will compute every time. */
        ;

    ensure_squarefree(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        bool same = squarefree[i] == squarefree[n - i];
        if (same)
            sum += 1;
    }
    seq_t result = (seq_t)n * sum;
    a_cache[n] = result;   /* store for future calls */
    return result;
}
