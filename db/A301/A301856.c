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
 * Helpers:   get_divisors_and_map, process_factorization
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
#include "../utils/get_divisors_and_map.h"
#include "../utils/process_factorization.h"

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
