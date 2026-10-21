/*
 * A265675  Number of smaller squarefree numbers that are coprime to the n-th
 *          squarefree number.
 * https://oeis.org/A265675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of smaller squarefree numbers that are coprime to
 *            the n‑th squarefree number
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

/* Cache of the first k squarefree numbers (1‑based indexing) */
static seq_t *sqfree = NULL;
static idx_t  sqfree_cnt = 0;   /* number of cached squarefree numbers   */
static idx_t  next_val   = 1;   /* next integer to test for squarefreeness */

/* Ensure at least n squarefree numbers are cached; on exit sqfree_cnt >= n */
static void
ensure_sqfree(idx_t n)
{
    if (sqfree_cnt >= n)
        return;

    /* Allocate or grow the cache */
    if (sqfree == NULL) {
        sqfree = (seq_t *)malloc(sizeof(seq_t) * n);
    } else {
        sqfree = (seq_t *)realloc(sqfree, sizeof(seq_t) * n);
    }

    /* Find further squarefree numbers until we have n of them */
    while (sqfree_cnt < n) {
        if (is_squarefree((seq_t)next_val)) {
            sqfree[sqfree_cnt++] = (seq_t)next_val;
        }
        ++next_val;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 0 by definition */
    if (n == 1)
        return 0;

    /* Make sure we have the first n squarefree numbers */
    ensure_sqfree(n);

    seq_t target = sqfree[n - 1];   /* n‑th squarefree number (0‑based index) */
    seq_t count  = 0;

    /* Count earlier squarefree numbers coprime to target */
    for (idx_t k = 0; k < n - 1; ++k) {
        if (gcd(target, sqfree[k]) == 1) {
            ++count;
        }
    }
    return count;
}
