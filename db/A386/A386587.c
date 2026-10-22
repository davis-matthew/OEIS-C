/*
 * A386587  Number of ways to choose a pairwise disjoint family of strict
 *          integer partitions, one of each exponent in the prime
 *          factorization of n.
 * https://oeis.org/A386587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to choose a pairwise‑disjoint family * of
 *            strict integer partitions, one of each exponent in * the prime
 *            factorization of n. * Uses recursive partition enumeration and
 *            backtracking
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility: array of partitions for a given exponent e
 *   each partition is stored as a 64‑bit mask indicating which parts
 *   are used.  The mask must fit in uint64_t because e ≤ 63 for uint64_t.
 */
typedef struct {
    uint64_t *masks;  /* array of masks */
    size_t count;     /* number of partitions */
} PartList;

static PartList partlists[65];
static int partlists_initialized = 0;

/* Ensure a dynamic array has room for at least need elements. */
static void ensure_capacity_uint64(uint64_t **arr, size_t *cnt, size_t *cap,
                                   size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Recursively generate all strict partitions of rem using parts ≤ max_part.
 * Each partition is represented as a bit‑mask where bit (p‑1) corresponds
 * to part p.  The current mask accumulates chosen parts.
 */
static void rec_partitions(int rem, int max_part, uint64_t mask,
                           uint64_t **arr, size_t *cnt, size_t *cap)
{
    if (rem == 0) {
        ensure_capacity_uint64(arr, cnt, cap, *cnt + 1);
        (*arr)[*cnt] = mask;
        (*cnt)++;
        return;
    }
    for (int part = (rem < max_part ? rem : max_part); part >= 1; --part) {
        int new_rem = rem - part;
        uint64_t new_mask = mask | ((uint64_t)1 << (part - 1));
        if (new_rem == 0) {
            ensure_capacity_uint64(arr, cnt, cap, *cnt + 1);
            (*arr)[*cnt] = new_mask;
            (*cnt)++;
        } else if (new_rem > 0 && part - 1 > 0) {
            rec_partitions(new_rem, part - 1, new_mask, arr, cnt, cap);
        }
    }
}

/* Build all partition lists up to exponent 64 once.  This is thread‑unsafe
 * but sufficient for the intended single‑threaded environment.
 */
static void init_partlists(void)
{
    if (partlists_initialized) return;
    for (int e = 1; e <= 64; ++e) {
        size_t cnt = 0, cap = 0;
        uint64_t *masks = NULL;
        rec_partitions(e, e, 0, &masks, &cnt, &cap);
        partlists[e].masks = masks;
        partlists[e].count = cnt;
    }
    partlists_initialized = 1;
}

/* Backtracking count of distinct‑part partitions for exponents ex[0..cnt-1].
 * 'used' keeps a bit‑mask of all integers already taken by earlier
 * partitions.  The function returns a seq_t count.
 */
static seq_t dfs_count(size_t idx, uint64_t used, const uint64_t *ex,
                       size_t cnt)
{
    if (idx == cnt) return (seq_t)1;
    seq_t total = 0;
    uint64_t e = ex[idx];
    const PartList *pl = &partlists[e];
    for (size_t j = 0; j < pl->count; ++j) {
        uint64_t m = pl->masks[j];
        if ((m & used) == 0) {
            total += dfs_count(idx + 1, used | m, ex, cnt);
        }
    }
    return total;
}

/* Prime factorization helper.  The function is defined in the utils
 * library with signature:
 *     void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt);
 */
#include "../utils/factor_u64.h"

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_partlists();

    /* Factor n; storage sufficient for all distinct primes of uint64_t. */
    size_t cnt = 0;
    uint64_t primes[16];
    uint64_t exponents[16];
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    if (cnt == 0) return (seq_t)1;   /* n == 1 */

    return dfs_count(0, 0, exponents, cnt);
}
