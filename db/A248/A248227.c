/*
 * A248227  Least k such that zeta(4) - sum{1/h^4, h = 1..k} < 1/n^3.
 * https://oeis.org/A248227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 368, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least k such that ζ(4) - Σ_{h=1}^k 1/h⁴ < 1/n³
 *
 * The computation is performed by maintaining the current tail
 * sum T(k) = Σ_{h=k+1}^∞ 1/h⁴ and incrementing k until the
 * inequality is satisfied.  For efficiency the function is memoised
 * and recomputes only new values when larger indices are requested.
 */

#include <stdbool.h>
#include <stdint.h>

/* The following utilities are available in the build environment.
 * We use seq_alloc_new_array for a dynamic array of seq_t values. */
#include "../utils/seq_alloc_new_array.h"

static seq_t *cache = NULL;          /* a_cache[i-1] = a(i) */
static size_t cache_cap = 0;         /* allocated capacity of cache      */
static idx_t max_n_computed = 0;     /* highest n for which a(n) known   */
static idx_t current_k = 0;          /* current tail index               */
static long double current_tail = 0; /* T(current_k) as long double      */
static bool initialized = false;     /* flag for one‑time initialisation */

/* Compute ζ(4) with long double precision.
 * The sum is performed until individual terms drop below 1e-25. */
static void init(void)
{
    /* Allocate cache for at least 10000 terms; can grow later. */
    cache_cap = 10000;
    cache = (seq_t *)seq_alloc_new_array(cache_cap);

    /* ζ(4) = Σ_{m=1}^∞ 1/m⁴ */
    long double sum = 0.0L;
    for (idx_t m = 1; m <= 1000000; ++m) {
        long double term = 1.0L /
                           ((long double)m * m * m * m);
        if (term < 1e-25L) break;
        sum += term;
    }
    current_tail = sum;  /* T(0) = ζ(4) */

    initialized = true;
}

seq_t a(idx_t n)
{
    /* Initialise once */
    if (!initialized) init();

    /* Return cached value if already computed */
    if (n <= max_n_computed) return cache[n - 1];

    /* Compute values for all indices up to n */
    for (idx_t idx = max_n_computed + 1; idx <= n; ++idx) {
        long double threshold = 1.0L /
                                ((long double)idx * idx * idx);

        /* Increase k until tail sum is strictly below the threshold */
        while (current_tail >= threshold) {
            ++current_k;
            current_tail -= 1.0L /
                           ((long double)current_k *
                            current_k *
                            current_k *
                            current_k);
        }

        /* Store result; seq_t can be constructed from idx_t */
        cache[idx - 1] = (seq_t)current_k;
    }

    max_n_computed = n;
    return cache[n - 1];
}
