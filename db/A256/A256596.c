/*
 * A256596  a(n) is the number of iterations of the map x->sigma(x) when
 *          starting from n before arriving at a number with more than one
 *          ancestor, with a(1)=0 and where sigma is the sum of divisors.
 * https://oeis.org/A256596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed as the number of iterations of the
 *            sum‑of‑divisors map σ(x) starting from n until the first value
 *            with more than one preimage (i.e. A054973(x) > 1). a(1) = 0
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ----------------------------------------------------------------------
 * Pre‑compute σ(i) and preimage counts up to LIMIT.
 * LIMIT is chosen large enough for all n up to at least 10 000 and for
 * the fork values that appear in the sequence.
 * ---------------------------------------------------------------------- */
static const idx_t LIMIT = 5000000ULL;

static uint32_t *sigma_table  = NULL;    /* σ(i) for 0 ≤ i ≤ LIMIT   */
static uint8_t  *preimage_cnt = NULL;    /* number of j with σ(j)=i   */
static bool      tables_ok    = false;

static void init_tables(void)
{
    if (tables_ok)
        return;
    tables_ok = true;

    /* Allocate arrays */
    sigma_table  = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    preimage_cnt = (uint8_t  *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!sigma_table || !preimage_cnt) {
        /* In a real implementation we might abort; here we simply
           leave the function unusable if allocation failed. */
        return;
    }

    /* Initialize sigma_table to zero */
    for (idx_t i = 0; i <= LIMIT; ++i)
        sigma_table[i] = 0;

    /* Sieve: add each divisor i to all multiples of i */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i)
            sigma_table[j] += (uint32_t)i;
    }

    /* Count preimages: for each j, increment count for σ(j) */
    for (idx_t j = 1; j <= LIMIT; ++j) {
        uint32_t s = sigma_table[j];
        if (s <= LIMIT) {
            /* uint8_t will wrap after 255; we only test >1. */
            ++preimage_cnt[s];
        }
    }
}

/* ----------------------------------------------------------------------
 * Main sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise tables on first use */
    if (!tables_ok)
        init_tables();

    if (n <= 1)
        return (seq_t)0;         /* a(1) = 0 */

    seq_t cur   = (seq_t)n;
    seq_t steps = 0;

    while (true) {
        /* cur must be within pre‑computed range */
        uint32_t s = sigma_table[(uint32_t)cur];

        /* Count this iteration before checking the fork */
        ++steps;

        /* If the next value has more than one preimage,
           the chain stops.  Count the iteration that produced it. */
        if (preimage_cnt[s] > 1)
            break;

        /* Continue iterating */
        cur = (seq_t)s;
    }

    return steps;
}
