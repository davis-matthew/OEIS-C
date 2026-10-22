/*
 * A328545  Number of 11-regular partitions of n (no part is a multiple of
 *          11).
 * https://oeis.org/A328545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of 11‑regular partitions of n (no part is a multiple of
 *            11)
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 132, u64 n <= 439,
 *            u128 n <= 1559, big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *p_cache = NULL;   /* table of unrestricted partition numbers p(n) */
static size_t p_cap   = 0;      /* number of entries currently stored */

/* Ensure that p_cache contains partition numbers p[0] … p[n] */
static void ensure_p_upto(idx_t n)
{
    if (p_cap > 0 && (idx_t)(p_cap - 1) >= n) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *newp = seq_alloc_new_array(new_cap);

    /* copy the old values (if any) */
    if (p_cache != NULL) {
        for (size_t i = 0; i < p_cap; ++i) newp[i] = p_cache[i];
        free_seq_array(p_cache);
    }

    if (p_cap == 0) {             /* first time: set p[0] = 1 */
        newp[0] = 1;
        p_cap = 1;
    }

    /* compute remaining entries using Euler’s recurrence */
    for (size_t i = p_cap; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if ((k & 1) == 1) sum += newp[i - pent1];
            else             sum -= newp[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if ((k & 1) == 1) sum += newp[i - pent2];
            else             sum -= newp[i - pent2];
        }
        newp[i] = sum;
    }

    p_cache = newp;
    p_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* make sure that all necessary partition numbers are available */
    ensure_p_upto(n);

    seq_t result = p_cache[n];            /* contribution from m = 0 */
    idx_t max_base = n / 11;              /* largest pentagonal number allowed */

    for (idx_t k = 1; ; ++k) {
        idx_t pent1 = k * (3 * k - 1) / 2;
        if (pent1 > max_base) break;
        idx_t shift = 11 * pent1;
        if ((k & 1) == 1) result -= p_cache[n - shift];
        else             result += p_cache[n - shift];

        idx_t pent2 = k * (3 * k + 1) / 2;
        if (pent2 > max_base) continue;
        shift = 11 * pent2;
        if ((k & 1) == 1) result -= p_cache[n - shift];
        else             result += p_cache[n - shift];
    }

    return result;
}
