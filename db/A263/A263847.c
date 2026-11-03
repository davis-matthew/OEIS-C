/*
 * A263847  a(n) = p(2*n)-p(2*n-2)-p(n) where p(n) are the partition numbers
 *          A000041(n).
 * https://oeis.org/A263847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(2*n) - p(2*n-2) - p(n) where p(k) is the partition
 *            number A000041(k)
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 72, u64 n <= 226,
 *            u128 n <= 771, big any n
 * Validated: standard, pass, 60 terms through n = 901 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partitions are computed with the Euler generalized pentagonal recurrence. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* Compute partitions up to 2*n if needed. */
    idx_t target = 2 * n;

    static seq_t *part = NULL;   /* array of partition values p[0..t] */
    static size_t cap = 0;      /* current allocated capacity */
    static idx_t maxComp = 0;   /* largest index already computed */

    /* Initial allocation and base case p(0) = 1. */
    if (part == NULL) {
        ensure_cache(1, &part, &cap);
        part[0] = (seq_t)1;
        maxComp  = 0;
    }

    /* Ensure storage for all needed indices. */
    if ((idx_t)(cap - 1) < target) {
        ensure_cache(target + 1, &part, &cap);
    }

    /* Compute missing partition numbers up to target. */
    if (maxComp < target) {
        for (idx_t m = maxComp + 1; m <= target; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1;; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > m) break;
                seq_t term = part[m - pent1];
                if (k & 1)      sum += term;   /* odd k: + */
                else            sum -= term;   /* even k: - */

                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > m) continue;
                term = part[m - pent2];
                if (k & 1)      sum += term;
                else            sum -= term;
            }
            part[m] = sum;
        }
        maxComp = target;
    }

    /* a(n) = p(2n) - p(2n-2) - p(n). */
    seq_t result = part[target];
    result -= part[target - 2];
    result -= part[n];
    return result;
}
