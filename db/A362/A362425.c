/*
 * A362425  Number of partitions of n into 3 distinct perfect powers
 *          (A001597).
 * https://oeis.org/A362425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of partitions of n into three distinct perfect powers.
   The implementation builds the list of distinct perfect powers ≤ n (with
   1 included) and, for each n, counts all unordered triples of distinct
   powers whose sum equals n.  Results are cached so that the function
   can be called repeatedly in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* External integer types provided by the build. */
/* idx_t   – unsigned integer type for indices (e.g. uint64_t)   */
/* seq_t   – unsigned integer type for sequence values (may be 8–128 bits) */

static idx_t        max_cached_n   = 0;          /* highest n cached */
static seq_t *      count_cache    = NULL;       /* a(k) for k ≤ max_cached_n */
static idx_t *      powers         = NULL;       /* distinct perfect powers ≤ max_cached_n */
static size_t      power_count     = 0;          /* number of stored powers */
static size_t      power_cap       = 0;          /* allocation capacity for powers */

/* Re‑compute the list of perfect powers and the counts array up to new_n. */
static void extend_cache(idx_t new_n)
{
    /* Allocate an array to mark which integers ≤ new_n are perfect powers. */
    size_t vis_len = (size_t)new_n + 1;
    uint8_t *visited = (uint8_t *)calloc(vis_len, 1);
    if (!visited) return;                 /* allocation failure – keep old cache */

    /* Create the list of distinct perfect powers.  Start with 1. */
    if (power_count == 0) {
        /* start with only the power 1 */
        power_cap = 4;
        powers = (idx_t *)malloc(power_cap * sizeof(idx_t));
        if (!powers) { free(visited); return; }
        powers[0] = 1;
        power_count = 1;
        visited[1] = 1;
    }

    /* Mark any existing powers that are ≤ new_n. */
    for (size_t i = 0; i < power_count; ++i) {
        idx_t p = powers[i];
        if (p <= new_n) visited[(size_t)p] = 1;
    }

    /* Generate new perfect powers (base ≥ 2, exponent ≥ 2). */
    for (idx_t base = 2; ; ++base) {
        /* base*base may overflow, so stop if it would exceed new_n. */
        if (base > new_n / base) break;          /* base^2 > new_n */
        idx_t power = base * base;               /* exponent 2 */
        while (power <= new_n) {
            if (!visited[(size_t)power]) {
                /* add power to the array */
                if (power_count == power_cap) {
                    power_cap = power_cap ? power_cap * 2 : 4;
                    idx_t *tmp = (idx_t *)realloc(powers, power_cap * sizeof(idx_t));
                    if (!tmp) { free(visited); return; }
                    powers = tmp;
                }
                powers[power_count++] = power;
                visited[(size_t)power] = 1;
            }
            /* Prepare for next exponent; check for overflow. */
            if (power > new_n / base) break;
            power *= base;
        }
        /* If even the smallest exponent is too large, no larger bases are needed. */
        if (base > new_n / base) break;
    }

    /* Allocate a new counts array and zero it. */
    seq_t *new_cache = (seq_t *)calloc(vis_len, sizeof(seq_t));
    if (!new_cache) { free(visited); return; }

    /* Count all unordered triples of distinct perfect powers. */
    for (size_t i = 0; i + 2 < power_count; ++i) {
        idx_t pi = powers[i];
        for (size_t j = i + 1; j + 1 < power_count; ++j) {
            idx_t pj = powers[j];
            idx_t sum2 = pi + pj;
            if (sum2 > new_n) break;            /* larger j will only increase sum2 */
            for (size_t k = j + 1; k < power_count; ++k) {
                idx_t pk = powers[k];
                idx_t sum3 = sum2 + pk;
                if (sum3 > new_n) break;        /* larger k only increases sum3 */
                ++new_cache[(size_t)sum3];
            }
        }
    }

    /* Replace the old cache with the new one. */
    free(count_cache);
    count_cache = new_cache;
    max_cached_n = new_n;

    free(visited);
}

/* Public sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n > max_cached_n) extend_cache(n);
    if (!count_cache) return 0;           /* safety in case of allocation failure */
    return count_cache[(size_t)n];
}
