/*
 * A301856  Number of subset-products (greater than 1) of factorizations of n
 *          into factors greater than 1.
 * https://oeis.org/A301856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 94 terms through n = 1000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A301856: number of subset-products (greater than 1) of all
   multiplicative factorizations of n into factors > 1.
   a(n) = Σ_{p ∈ factorizations of n} |{ product of a nonempty submultiset of p }|
*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Build list of divisors >1 of n and a mapping from divisor to index */
static void get_divisors_and_map(idx_t n, idx_t **divs, size_t *divcnt, int **map)
{
    size_t cap = 16;
    *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    *divcnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (n % i == 0) {
            if (*divcnt == cap) {
                cap <<= 1;
                *divs = (idx_t *)realloc(*divs, cap * sizeof(idx_t));
            }
            (*divs)[(*divcnt)++] = i;
        }
    }

    *map = (int *)malloc((n + 1) * sizeof(int));
    for (idx_t i = 0; i <= n; ++i)
        (*map)[i] = -1;
    for (size_t i = 0; i < *divcnt; ++i)
        (*map)[(*divs)[i]] = (int)i;
}

/* Count distinct subset-product t (>1) for a given factorization */
static void process_factorization(const idx_t *factors, size_t depth,
                                  const idx_t *divs, size_t divcnt,
                                  const int *map, seq_t *total)
{
    unsigned char *visited = (unsigned char *)calloc(divcnt, sizeof(unsigned char));
    if (!visited) exit(1);

    size_t subset_count = ((size_t)1 << depth);
    for (size_t mask = 1; mask < subset_count; ++mask) {
        idx_t prod = 1;
        for (size_t j = 0; j < depth; ++j) {
            if (mask & ((size_t)1 << j))
                prod *= factors[j];
        }
        int di = map[prod];
        if (di >= 0)
            visited[di] = 1;
    }

    seq_t cnt = 0;
    for (size_t i = 0; i < divcnt; ++i)
        if (visited[i]) ++cnt;

    *total += cnt;
    free(visited);
}

static void dfs(idx_t rem, idx_t min_factor,
                idx_t *factors, size_t depth,
                const idx_t *divs, size_t divcnt,
                const int *map, seq_t *total);

static void process_factorization(const idx_t *factors, size_t depth,
                                  const idx_t *divs, size_t divcnt,
                                  const int *map, seq_t *total);

static void get_divisors_and_map(idx_t n, idx_t **divs, size_t *divcnt, int **map);

/* Main entry */
seq_t a(idx_t n) {
    if (n <= 1) return 0;

    idx_t *divs = NULL;
    size_t divcnt = 0;
    int *map = NULL;
    get_divisors_and_map(n, &divs, &divcnt, &map);

    seq_t total = 0;
    idx_t factors[32];  /* maximal depth is log2(n) + 1 for n <= 10000 */
    dfs(n, 2, factors, 0, divs, divcnt, map, &total);

    free(divs);
    free(map);
    return total;
}

/* Recursively enumerate multiplicative factorizations of rem */
static void dfs(idx_t rem, idx_t min_factor,
                idx_t *factors, size_t depth,
                const idx_t *divs, size_t divcnt,
                const int *map, seq_t *total)
{
    for (idx_t f = min_factor; f * f <= rem; ++f) {
        if (rem % f == 0) {
            factors[depth++] = f;
            dfs(rem / f, f, factors, depth, divs, divcnt, map, total);
            --depth;
        }
    }
    if (rem >= min_factor) {
        factors[depth++] = rem;
        process_factorization(factors, depth, divs, divcnt, map, total);
        --depth;
    }
}
