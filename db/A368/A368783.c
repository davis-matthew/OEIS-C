/*
 * A368783  Lexicographic rank of the permutation which is the Eytzinger array
 *          layout of n elements.
 * https://oeis.org/A368783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographic rank of the permutation that is the
 *            Eytzinger array layout of n elements (a(0)=0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   bit_sum, bit_update, dfs_assign
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/bit_sum.h"
#include "../utils/bit_update.h"
#include "../utils/dfs_assign.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Static caches for successive calls. */
    static idx_t *perm_cache = NULL;
    static size_t perm_cap = 0;

    static int *bit_cache = NULL;
    static size_t bit_cap = 0;

    static seq_t *fact_cache = NULL;
    static size_t fact_cap = 0;

    /* Resize permutation array if necessary. */
    if (n > perm_cap) {
        perm_cap = n;
        perm_cache = (idx_t *)realloc(perm_cache, (perm_cap + 1) * sizeof(idx_t));
    }

    /* Resize factorial table if necessary and compute from scratch. */
    if (n > fact_cap) {
        fact_cap = n;
        fact_cache = (seq_t *)realloc(fact_cache, (fact_cap + 1) * sizeof(seq_t));
        fact_cache[0] = 1;
        for (size_t i = 1; i <= fact_cap; ++i)
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }

    /* Resize BIT array if necessary. */
    if (n > bit_cap) {
        bit_cap = n;
        bit_cache = (int *)realloc(bit_cache, (bit_cap + 2) * sizeof(int));
    }

    /* Clear BIT (entries 1..n) and initialise with all 1s. */
    memset(bit_cache, 0, (n + 2) * sizeof(int));
    for (size_t i = 1; i <= n; ++i)
        bit_update(bit_cache, n, i, 1);

    /* Build Eytzinger permutation. */
    idx_t next_val = 1;
    dfs_assign(1, n, perm_cache, &next_val);

    /* Compute lexicographic rank. */
    seq_t rank = 0;
    for (size_t pos = 1; pos <= n; ++pos) {
        idx_t val = perm_cache[pos];
        int smaller_unused = bit_sum(bit_cache, (size_t)(val - 1));
        seq_t term = fact_cache[n - pos];
        term = term * (seq_t)smaller_unused;
        rank += term;
        bit_update(bit_cache, n, val, -1);  /* mark value as used */
    }

    return rank;
}
