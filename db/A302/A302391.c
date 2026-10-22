/*
 * A302391  Number of partitions of 2n into two parts with at least one
 *          nonsquarefree part.
 * https://oeis.org/A302391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Σ_{i=1..n} μ(i)² · μ(2n‑i)² μ²(k) = 1 if k is
 *            square‑free, otherwise 0. Thus a(n) counts i in [1,n] for which
 *            at least one of i or 2n‑i is not square‑free
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *sqfree = NULL;   /* sqfree[i]==1 iff i is square‑free */
static idx_t          cap    = 0;      /* highest index that has been filled */

static void ensure_sqfree(idx_t limit)
{
    if (limit <= cap)
        return;

    /* Allocate or reallocate the table */
    unsigned char *new_tbl = (unsigned char *)malloc((size_t)(limit + 1));
    if (!new_tbl)
        return;                       /* out‑of‑memory → leave function in
                                          undefined state; caller will get 0  */

    /* Initially assume everything is square‑free */
    for (idx_t i = 0; i <= limit; ++i)
        new_tbl[i] = 1;

    /* Mark non‑square‑free numbers by marking multiples of p²
       for each prime p ≤ √limit.  We generate primes by trial
       division – the cost is negligible for the sizes involved.   */
    idx_t sqrt_lim = 0;
    while ((sqrt_lim + 1) * (sqrt_lim + 1) <= limit)
        ++sqrt_lim;

    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        /* Test whether p is prime */
        bool is_prime = true;
        for (uint64_t d = 2; d * d <= (uint64_t)p; ++d) {
            if (p % (idx_t)d == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        uint64_t sq = (uint64_t)p * (uint64_t)p;
        for (uint64_t j = sq; j <= limit; j += sq)
            new_tbl[(size_t)j] = 0;
    }

    /* Replace the old table, freeing memory */
    free(sqfree);
    sqfree = new_tbl;
    cap    = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* behaviour below the offset is unspecified */

    idx_t limit = 2 * n;
    ensure_sqfree(limit);

    seq_t squarefree_both = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (sqfree[i] && sqfree[limit - i])
            ++squarefree_both;
    }

    return (seq_t)n - squarefree_both;
}
