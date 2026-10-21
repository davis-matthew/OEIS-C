/*
 * A280634  Number of partitions of 2n into two refactorable parts.
 * https://oeis.org/A280634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # { 1 ≤ i ≤ n | i is refactorable and 2n − i is
 *            refactorable } Refactorable: d(x) divides x, where d(x) is the
 *            number of divisors of x
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The divisor count routine used here is supplied in "../utils/divisor_count.h". */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/* Function to count divisors of a 64‑bit integer (provided by the build). */
uint64_t divisor_count_u64(uint64_t n);

/* --- divisor‑count cache ---------------------------------------------------- */
static uint32_t *divcnt_cache = NULL;   /* divisor counts [0 … size-1]            */
static idx_t     divcnt_size  = 0;       /* current cache capacity                */

/* Ensure that divcnt_cache contains divisor counts up to and including 'up_to'.
   The cache is rebuilt from scratch each time to keep the code very small.     */
static void
ensure_divcnt_cache(idx_t up_to)
{
    if (up_to + 1 <= divcnt_size)
        return;                         /* already large enough */

    /* Allocate new cache. */
    idx_t new_size = up_to + 1;
    uint32_t *new_cache = (uint32_t *)malloc(new_size * sizeof(uint32_t));
    if (!new_cache)
        return;                         /* abort on allocation failure */

    /* Zero initialise the whole cache. */
    memset(new_cache, 0, new_size * sizeof(uint32_t));

    /* Fill divisor counts by the classic sieve approach. */
    for (idx_t i = 1; i <= up_to; ++i) {
        for (idx_t j = i; j <= up_to; j += i)
            ++new_cache[j];
    }

    /* Replace old cache. */
    free(divcnt_cache);
    divcnt_cache = new_cache;
    divcnt_size  = new_size;
}

/* ----------------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)                       /* offset is 1; return 0 for safety */
        return (seq_t)0;

    idx_t two_n = n + n;              /* 2n fits into idx_t for all test ranges */

    ensure_divcnt_cache(two_n);       /* make sure we have divisor counts */

    seq_t count = 0;                  /* result to be returned */

    for (idx_t i = 1; i <= n; ++i) {
        /* Check if i is refactorable. */
        if (i % divcnt_cache[i] != 0)
            continue;

        idx_t j = two_n - i;          /* second part of the partition   */
        /* Check if the second part is refactorable. */
        if (j % divcnt_cache[j] != 0)
            continue;

        ++count;                      /* both parts refactorable => count */
    }
    return count;
}
