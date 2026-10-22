/*
 * A322436  Number of pairs of factorizations of n into factors > 1 where no
 *          factor of the second properly divides any factor of the first.
 * https://oeis.org/A322436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322436: number of pairs of factorizations of n (into factors > 1)
   where no factor of the second factorization properly divides any
   factor of the first.  The function enumerates all such factorizations
   and counts the qualifying pairs.  */

/* Standard headers – the build supplies seq_t and idx_t. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Data structure for a factorization: array of factors and its length. */
typedef struct {
    idx_t *factors;
    size_t len;
} fact_t;

/* Recursive helper that builds all factorizations of n using
   factors ≥ min_d (non‑decreasing order). */
static void build_factorizations(idx_t n, idx_t min_d, idx_t *cur, size_t depth,
                                 fact_t **list, size_t *len, size_t *cap)
{
    /* Find all proper divisors d of n that are at least min_d. */
    for (idx_t d = min_d; d * d <= n; ++d) {
        if (n % d == 0) {
            cur[depth] = d;
            build_factorizations(n / d, d, cur, depth + 1, list, len, cap);
        }
    }
    /* Store the factorization cur[0..depth-1] followed by n itself. */
    fact_t f;
    f.len = depth + 1;
    f.factors = (idx_t *)malloc(f.len * sizeof(idx_t));
    if (f.factors == NULL) exit(EXIT_FAILURE);
    if (depth > 0)
        memcpy(f.factors, cur, depth * sizeof(idx_t));
    f.factors[depth] = n;

    /* Resize storage array if needed. */
    if (*len == *cap) {
        *cap = (*cap == 0) ? 4 : *cap * 2;
        *list = (fact_t *)realloc(*list, *cap * sizeof(fact_t));
        if (*list == NULL) exit(EXIT_FAILURE);
    }
    (*list)[*len] = f;
    (*len)++;
}

/* Checks that every factor of G does not properly divide any factor of F. */
static bool valid_pair(const fact_t *F, const fact_t *G)
{
    for (size_t j = 0; j < G->len; ++j) {
        idx_t b = G->factors[j];
        for (size_t i = 0; i < F->len; ++i) {
            idx_t a = F->factors[i];
            if (a % b == 0 && a != b)
                return false;
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Simple memoisation of previously computed values. */
    static idx_t *cache_n   = NULL;
    static seq_t *cache_val = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Return cached result if available. */
    for (size_t i = 0; i < cache_len; ++i) {
        if (cache_n[i] == n) return cache_val[i];
    }

    seq_t result = 0;

    if (n == 1) {
        /* Only the empty factorisation pair (1 | 1). */
        result = 1;
    } else {
        /* Enumerate all factorizations of n. */
        fact_t *factorizations = NULL;
        size_t fact_len = 0, fact_cap = 0;
        idx_t cur[32];  /* enough for n <= 2^31; adjust if needed. */
        build_factorizations(n, 2, cur, 0, &factorizations, &fact_len, &fact_cap);

        /* Count directed pairs (F, G) that satisfy the divisor condition. */
        seq_t count = 0;
        for (size_t i = 0; i < fact_len; ++i) {
            for (size_t j = 0; j < fact_len; ++j) {
                if (valid_pair(&factorizations[i], &factorizations[j]))
                    count++;
            }
        }
        result = count;

        /* Clean up. */
        for (size_t i = 0; i < fact_len; ++i) free(factorizations[i].factors);
        free(factorizations);
    }

    /* Store computed value in the cache. */
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        cache_n = (idx_t *)realloc(cache_n, cache_cap * sizeof(idx_t));
        if (cache_n == NULL) exit(EXIT_FAILURE);
        cache_val = (seq_t *)realloc(cache_val, cache_cap * sizeof(seq_t));
        if (cache_val == NULL) exit(EXIT_FAILURE);
    }
    cache_n[cache_len] = n;
    cache_val[cache_len] = result;
    cache_len++;

    return result;
}
