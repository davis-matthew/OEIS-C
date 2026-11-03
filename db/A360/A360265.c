/*
 * A360265  a(0) = 0, and for any n > 0, let k > 0 be as small as possible and
 *          such that t(k) >= n (where t(m) denotes A000217(m), the m-th
 *          triangular number); a(n) = k + a(t(k) - n).
 * https://oeis.org/A360265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0; for n>0 let k be the smallest positive integer with
 *            k(k+1)/2 ≥ n, then a(n) = k + a(k(k+1)/2 – n)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9518 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *cache   = NULL;   /* stored values a(i) */
static bool *computed = NULL;   /* computed[i] == true if a(i) is stored */
static size_t cap = 0;          /* maximum index already allocated (inclusive) */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold index n */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n + 1;              /* array size */
        seq_t *new_cache   = mem_new(seq_t, new_cap);
        bool *new_computed = (bool  *)malloc(new_cap * sizeof(bool));
        if (cache) {
            for (size_t i = 0; i <= cap; ++i) {
                new_cache[i]   = cache[i];
                new_computed[i] = computed[i];
            }
            mem_free(cache);
            free(computed);
        }
        /* initialise the new slots */
        for (size_t i = cap + 1; i <= n; ++i) {
            new_cache[i]   = 0;
            new_computed[i] = false;
        }
        cache    = new_cache;
        computed = new_computed;
        cap      = (size_t)n;
    }

    if (n == 0) return 0;
    if (computed[n]) return cache[n];

    /* find the smallest k with k(k+1)/2 >= n */
    idx_t k = 1;
    while ((seq_t)k * (k + 1) / 2 < n) ++k;

    seq_t t = (seq_t)k * (k + 1) / 2;   /* triangular number t(k) */
    idx_t r = (idx_t)(t - n);           /* remainder in [0, k-1] */

    seq_t result = (seq_t)k + a(r);
    cache[n] = result;
    computed[n] = true;
    return result;
}
