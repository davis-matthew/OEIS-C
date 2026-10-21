/*
 * A292886  Number of knapsack factorizations of n.
 * https://oeis.org/A292886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of knapsack factorizations of n
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   generate_products
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/generate_products.h"

/* Comparator for qsort of idx_t values. */
static int cmp_idx_t(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    return (*x > *y) - (*x < *y);
}

/* Determine whether the multiset of factors given in parts[0..depth-1]
   satisfies the knapsack property. */
static bool is_knapsack(const uint32_t *parts, int depth)
{
    /* Build arrays of unique values and their multiplicities. */
    uint32_t vals[64];
    int freqs[64];
    int uniq = 0;
    for (int i = 0; i < depth; i++) {
        uint32_t v = parts[i];
        int j = 0;
        while (j < uniq && vals[j] != v)
            j++;
        if (j == uniq) {
            vals[uniq] = v;
            freqs[uniq] = 1;
            uniq++;
        } else {
            freqs[j]++;
        }
    }

    /* Collect all distinct subset products. */
    static idx_t prods[1 << 20];   /* large enough for n ≤ 10000 */
    int ptr = 0;
    generate_products(0, uniq, vals, freqs, (seq_t)1, prods, &ptr);

    /* Check for duplicate products. */
    qsort(prods, ptr, sizeof(idx_t), cmp_idx_t);
    for (int i = 1; i < ptr; i++) {
        if (prods[i] == prods[i - 1])
            return false;
    }
    return true;
}

/* Recursive enumeration of multiplicative partitions of n. */
static void dfs(idx_t n, idx_t min_fac, uint32_t *parts, int depth, seq_t *cnt)
{
    if (n == 1) {
        if (is_knapsack(parts, depth))
            (*cnt)++;
        return;
    }

    for (idx_t f = min_fac; f * f <= n; ++f) {
        if (n % f == 0) {
            parts[depth] = (uint32_t)f;
            dfs(n / f, f, parts, depth + 1, cnt);
        }
    }

    /* Consider the factor n itself. */
    if (min_fac <= n) {
        parts[depth] = (uint32_t)n;
        dfs(1, n, parts, depth + 1, cnt);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint32_t parts[64];
    seq_t result = 0;
    dfs(n, 2, parts, 0, &result);
    return result;
}
