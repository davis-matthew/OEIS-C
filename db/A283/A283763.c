/*
 * A283763  Number of ways of writing n as sum of a deficient number (A005100)
 *          and abundant number (A005101).
 * https://oeis.org/A283763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of representations n = d + a with d a deficient
 *            number (A005100) and a an abundant number (A005101)
 * Range:     u8 n <= 1050, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global dynamic tables */
static idx_t   max_precomp = 0;          /* largest n already prepared      */
static uint8_t *def_flag  = NULL;        /* 1 if i is deficient             */
static uint8_t *abund_flag = NULL;      /* 1 if i is abundant              */
static uint64_t *sigma = NULL;          /* sum of proper divisors of i     */

/* Allocate or grow tables to contain all numbers up to new_max (inclusive). */
static void ensure_precomputation(idx_t new_max)
{
    if (new_max <= max_precomp)
        return;

    size_t new_size = (size_t)new_max + 1;

    /* reallocate with explicit casts to satisfy C++ */
    def_flag   = (uint8_t*)realloc(def_flag,   new_size * sizeof(uint8_t));
    abund_flag = (uint8_t*)realloc(abund_flag, new_size * sizeof(uint8_t));
    sigma      = (uint64_t*)realloc(sigma,      new_size * sizeof(uint64_t));
    if (!def_flag || !abund_flag || !sigma)
        return;        /* leave tables untouched on failure            */

    /* initialise the new sigma array */
    memset(sigma, 0, new_size * sizeof(uint64_t));

    /* compute σ(n): sum of proper divisors for 1 ≤ n ≤ new_max */
    for (idx_t i = 1; i <= new_max; ++i) {
        for (idx_t j = i + i; j <= new_max; j += i)
            sigma[j] += i;
    }

    /* classify each number as deficient (σ < n) or abundant (σ > n) */
    for (idx_t i = 1; i <= new_max; ++i) {
        def_flag[i]   = (sigma[i] < (uint64_t)i) ? 1 : 0;
        abund_flag[i] = (sigma[i] > (uint64_t)i) ? 1 : 0;
    }

    max_precomp = new_max;
}

/* Return the number of representations of n as a deficient + abundant pair */
seq_t a(idx_t n)
{
    if (n == 0)               /* by definition a(0) = 0 */
        return (seq_t)0;

    /* make sure all needed information is available */
    ensure_precomputation(n);

    seq_t count = (seq_t)0;
    for (idx_t d = 1; d < n; ++d) {
        if (def_flag[d] && abund_flag[n - d])
            ++count;
    }
    return count;
}
