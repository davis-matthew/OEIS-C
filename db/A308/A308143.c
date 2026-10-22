/*
 * A308143  Take all the integer-sided triangles with perimeter n and
 *          squarefree sides a, b, and c such that a <= b <= c. a(n) is the
 *          sum of all the b's.
 * https://oeis.org/A308143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of b over all integer‑sided triangles with perimeter
 *            n whose sides a, b, c are squarefree and satisfy a ≤ b ≤ c
 * Range:     u8 n <= 44, u16 n <= 332, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char *squarefree_cache = NULL;
static idx_t squarefree_cache_size = 0;

/* Compute the squarefree sieve up to n (inclusive) and store in
   squarefree_cache.  squarefree_cache[i] == 1  ⇔  i is squarefree. */
static void init_squarefree(idx_t n)
{
    if (squarefree_cache_size >= n + 1)
        return;

    if (squarefree_cache != NULL)
        free(squarefree_cache);

    squarefree_cache = (unsigned char*)malloc((n + 1) * sizeof(unsigned char));
    for (idx_t i = 0; i <= n; ++i)
        squarefree_cache[i] = 1;        /* assume squarefree */

    for (idx_t i = 2; i * i <= n; ++i) {
        if (squarefree_cache[i]) {           /* i currently squarefree */
            idx_t sq = i * i;
            for (idx_t j = sq; j <= n; j += sq)
                squarefree_cache[j] = 0;      /* multiple of a square */
        }
    }
    squarefree_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    if (n < 3)          /* no triangle possible */
        return sum;

    init_squarefree(n);

    idx_t half = n / 2;
    idx_t max_a = n / 3;                     /* a ≤ n/3 */

    for (idx_t a = 1; a <= max_a; ++a) {
        if (!squarefree_cache[a])
            continue;                        /* a not squarefree */

        idx_t min_b = a;
        if (a <= half) {
            idx_t candidate = half - a + 1;   /* ensures a + b > c */
            if (candidate > min_b)
                min_b = candidate;
        }

        idx_t max_b = (n - a) / 2;           /* ensures b ≤ c */
        if (min_b > max_b)
            continue;

        for (idx_t b = min_b; b <= max_b; ++b) {
            if (!squarefree_cache[b])
                continue;                    /* b not squarefree */

            idx_t c = n - a - b;
            if (!squarefree_cache[c])
                continue;                    /* c not squarefree */

            sum += (seq_t)b;                  /* triangle found */
        }
    }
    return sum;
}
