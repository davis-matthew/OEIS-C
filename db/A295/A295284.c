/*
 * A295284  Number of partitions of n into two distinct parts such that the
 *          larger part is nonsquarefree.
 * https://oeis.org/A295284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers L such that n/2 < L < n and L is * not
 *            squarefree. That equals ∑_{i=1}^{⌊(n−1)/2⌋} (1 − μ(n−i)^2) *
 *            where μ is the Möbius function
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Cache of squarefree status: true if the index is squarefree,
 * false otherwise.  We maintain it for all indices up to the
 * current capacity. */
static bool *squarefree = NULL;
static size_t squarefree_cap = 0;

/* Ensure that squarefree[] covers the range [0, n−1].
 * On first call or when a larger n is requested the table is
 * rebuilt.  The rebuild runs a simple sieve to mark multiples
 * of prime squares as non‑squarefree. */
static void ensure_squarefree(idx_t n)
{
    if ((size_t)n <= squarefree_cap)
        return;

    size_t new_cap = (size_t)n * 2 + 10;
    bool *new_sf = (bool *)realloc(squarefree, (new_cap + 1) * sizeof(bool));
    if (!new_sf)
        exit(1);
    for (size_t i = 0; i <= new_cap; ++i)
        new_sf[i] = true;

    /* Sieve primes up to sqrt(new_cap) and use them to mark
     * non‑squarefree numbers. */
    size_t limit = (size_t)sqrt((double)new_cap);
    bool *is_comp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_comp)
        exit(1);
    for (size_t i = 0; i <= limit; ++i)
        is_comp[i] = false;
    if (limit >= 0) {
        is_comp[0] = true;
        if (limit >= 1) is_comp[1] = true;
    }
    for (size_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (size_t j = p * p; j <= limit; j += p)
                is_comp[j] = true;
        }
    }
    for (size_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            size_t sq = p * p;
            for (size_t m = sq; m <= new_cap; m += sq)
                new_sf[m] = false;
        }
    }
    free(is_comp);

    squarefree = new_sf;
    squarefree_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Cover indices up to n−1 for the current n. */
    ensure_squarefree(n - 1);

    idx_t start = n / 2 + 1;          /* first larger part */
    seq_t count = 0;
    for (idx_t L = start; L <= n - 1; ++L) {
        if (!squarefree[(size_t)L])
            ++count;
    }
    return count;
}
