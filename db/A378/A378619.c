/*
 * A378619  Distance between n and the greatest squarefree number <= n.
 * https://oeis.org/A378619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (greatest squarefree number ≤ n)
 * Range:     u8 n <= 9754, u16 n <= 9754, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Static cache of squarefree flags (1 = squarefree, 0 = not).  Index 0 unused.  */
static unsigned char *sqfree_cache = NULL;   /* cache array */
static idx_t cache_max = 0;                  /* largest index currently cached (inclusive) */

/* Ensure that sqfree_cache contains correct flags for all indices up to n.  */
static void ensure_sqfree(idx_t n)
{
    if (n <= cache_max) return;          /* already up to date */

    idx_t newsize = n + 1;               /* include index n */
    unsigned char *tmp = (unsigned char *)realloc(sqfree_cache, newsize * sizeof(unsigned char));
    if (!tmp) exit(1);                   /* allocation failed */
    sqfree_cache = tmp;

    /* Initialize all to 1 (squarefree).  Index 0 is unused.  */
    for (idx_t i = 1; i < newsize; ++i) sqfree_cache[i] = 1;
    sqfree_cache[0] = 0;                 /* safety */

    /* Mark multiples of all squares d*d (d ≥ 2) as not squarefree. */
    idx_t limit = (idx_t)sqrt((double)(newsize - 1));
    for (idx_t d = 2; d <= limit; ++d) {
        idx_t sq = d * d;
        for (idx_t j = sq; j < newsize; j += sq) sqfree_cache[j] = 0;
    }

    cache_max = n;
}

/* Distance between n and the greatest squarefree number ≤ n.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* outside defined domain, safe to return 0 */

    ensure_sqfree(n);

    idx_t candidate = n;
    while (candidate >= 1 && !sqfree_cache[candidate]) {
        --candidate;
    }
    /* candidate is now the greatest squarefree ≤ n (must exist, at least 1). */
    seq_t result = (seq_t)(n - candidate);
    return result;
}
