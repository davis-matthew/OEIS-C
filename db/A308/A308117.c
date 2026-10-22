/*
 * A308117  Number of acute integer-sided triangles with perimeter n and
 *          squarefree sides.
 * https://oeis.org/A308117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of acute integer‑sided triangles with perimeter n
 *            * and all sides squarefree. See OEIS A308117
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to build the squarefree table up to at least n. */
static uint8_t *squarefree_table = NULL;
static size_t squarefree_len = 0;

static void
ensure_squarefree(idx_t n)
{
    if (squarefree_len >= n + 1)
        return;               /* already has enough space */

    /* Allocate a new table of length n+1. */
    uint8_t *tbl = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!tbl)
        abort();              /* allocation failure */

    /* Initially mark everything as squarefree. */
    for (idx_t i = 0; i <= n; ++i)
        tbl[i] = 1;
    tbl[0] = 0;               /* 0 is not squarefree (unused) */

    /* Sieve: mark numbers divisible by a square of a prime. */
    for (idx_t p = 2; (p * p) <= n; ++p) {
        if (tbl[p]) {          /* p is prime */
            idx_t sq = p * p;
            for (idx_t j = sq; j <= n; j += sq)
                tbl[j] = 0;    /* not squarefree */
        }
    }

    /* Replace old table. */
    free(squarefree_table);
    squarefree_table = tbl;
    squarefree_len = n + 1;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;       /* sequence defined for n >= 1 */

    /* Prepare the squarefree table up to n. */
    ensure_squarefree(n);

    idx_t count = 0;

    /* Enumerate all unordered triples (k, i, j) with
     * k <= i <= j, k + i + j = n, triangle inequality,
     * acute condition, and all sides squarefree. */
    for (idx_t k = 1; k <= n / 3; ++k) {
        for (idx_t i = k; i <= (n - k) / 2; ++i) {
            idx_t j = n - k - i;
            if (j < i)          /* ordering violated (shouldn't happen) */
                break;

            /* Triangle inequality: k + i > j . */
            if (k + i <= j)
                continue;

            /* Squarefree check. */
            if (!squarefree_table[k] || !squarefree_table[i] || !squarefree_table[j])
                continue;

            /* Acute condition for largest side j:
               j^2 < i^2 + k^2. */
            idx_t lhs = j * j;
            idx_t rhs = i * i + k * k;
            if (lhs >= rhs)
                continue;

            ++count;
        }
    }

    return (seq_t)count;
}
