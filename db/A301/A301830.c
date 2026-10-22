/*
 * A301830  Number of factorizations of n into factors (greater than 1) of two
 *          kinds.
 * https://oeis.org/A301830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over unordered factorizations of n into factors >1 of
 *            the product over distinct factor values of (count_of_value + 1).
 *            Equivalently, for each unordered multiset of factors whose
 *            product is n, multiply (c+1) for each distinct factor appearing
 *            c times
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/dfs_large.h"

/* ------------------------------------------------------------------ */
/* Pre‑compute divisors for numbers ≤ MAX_N (used for small n). */
/* Adjust MAX_N to a reasonable size for the target problem. */
#ifndef MAX_N
#define MAX_N 20000
#endif
#ifndef MAX_DIVS
#define MAX_DIVS 256
#endif

static bool built_divs = false;
static uint16_t divs[MAX_N + 1][MAX_DIVS];
static uint16_t dcnt[MAX_N + 1];

/* Cache for computed a(n) (only for n ≤ MAX_N) */
static bool seen[MAX_N + 1];
static seq_t cache[MAX_N + 1];

/* ------------------------------------------------------------------ */
/* Build divisor lists up to MAX_N (called once) */
static void build_divisors(void)
{
    if (built_divs) return;
    for (size_t i = 0; i <= (size_t)MAX_N; ++i) dcnt[i] = 0;
    for (uint64_t i = 2; i <= (uint64_t)MAX_N; ++i) {
        for (uint64_t j = i; j <= (uint64_t)MAX_N; j += i) {
            if (dcnt[j] < MAX_DIVS)
                divs[j][dcnt[j]++] = (uint16_t)i;
        }
    }
    built_divs = true;
}

/* ------------------------------------------------------------------ */
/* recursion for numbers ≤ MAX_N using pre‑computed divisors */
static void dfs_small(uint64_t n, uint64_t min_factor,
                      seq_t weight, uint64_t last_factor,
                      uint8_t last_count, seq_t *res)
{
    if (n == 1) {                /* finished a factorization */
        if (last_count > 0) weight *= (seq_t)(last_count + 1);
        *res += weight;
        return;
    }
    uint16_t nd = dcnt[(size_t)n];
    for (uint16_t i = 0; i < nd; ++i) {
        uint64_t d = divs[(size_t)n][i];
        if (d < min_factor) continue;
        if (d == last_factor) {
            dfs_small(n / d, d, weight, last_factor,
                      last_count + 1, res);
        } else {
            seq_t new_weight = weight * (seq_t)(last_count + 1);
            dfs_small(n / d, d, new_weight, d, 1, res);
        }
    }
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;        /* offset 1 */

    /* handle small n with caching */
    if (n <= (idx_t)MAX_N) {
        if (seen[(size_t)n]) return cache[(size_t)n];
        build_divisors();
        seq_t result = 0;
        dfs_small((uint64_t)n, 2, (seq_t)1, 0, 0, &result);
        seen[(size_t)n] = true;
        cache[(size_t)n] = result;
        return result;
    }

    /* handle larger n directly */
    seq_t result = 0;
    dfs_large((uint64_t)n, 2, (seq_t)1, 0, 0, &result);
    return result;
}
