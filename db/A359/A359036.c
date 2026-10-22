/*
 * A359036  a(1) = 1. Thereafter a(n) is the least unused k distinct from n
 *          such that d(k) = d(n), where d is the divisor counting function,
 *          A000005.
 * https://oeis.org/A359036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The permutation a(n) maps each n to the least unused k≠n with
 *            the same number of divisors d(k)=d(n). The mapping pairs
 *            consecutive numbers with equal divisor count in increasing order
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   make_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/make_seq_t.h"

/* internal storage */
static bool initialized = false;
static idx_t computed_max = 0;
static seq_t *perm = NULL;              /* permutation table */
static uint32_t *dcount = NULL;         /* divisor counts */
static uint32_t *first_unpaired = NULL; /* pending element per divisor count */

/* allocate and initialise tables up to limit (inclusive) */
static void init_up_to(idx_t limit)
{
    /* free old tables if they exist */
    free(perm);
    free(dcount);
    free(first_unpaired);

    /* allocate new tables */
    size_t sz = (size_t)limit + 1;
    dcount          = (uint32_t *)calloc(sz, sizeof(uint32_t));
    perm            = (seq_t *)calloc(sz, sizeof(seq_t));
    first_unpaired  = (uint32_t *)calloc(sz, sizeof(uint32_t));

    /* compute divisor counts: d(n) = number of positive divisors of n */
    for (size_t i = 1; i <= limit; ++i)
        for (size_t j = i; j <= limit; j += i)
            ++dcount[j];

    /* build the permutation by pairing consecutive elements of each
       divisor‑count class in increasing order */
    for (size_t k = 2; k <= limit; ++k) {
        uint32_t d = dcount[k];
        uint32_t pend = first_unpaired[d];
        if (pend == 0) {
            /* no pending element yet */
            first_unpaired[d] = (uint32_t)k;
        } else {
            /* pair k with its pending partner */
            perm[k] = make_seq_t(pend);
            perm[pend] = make_seq_t(k);
            first_unpaired[d] = 0;
        }
    }

    /* self‑map for n = 1 */
    perm[1] = make_seq_t(1);

    computed_max = limit;
    initialized = true;
}

/* ensure we have tables large enough to answer a(n) for all n up to need */
static void ensure_limit(idx_t need)
{
    if (!initialized || need > computed_max) {
        /* extend to a safe range: at least double plus a buffer */
        idx_t new_limit = need * 2 + 1000;
        if (new_limit <= need) new_limit = SIZE_MAX; /* guard against overflow */
        init_up_to(new_limit);
    }
}

seq_t a(idx_t n)
{
    /* lazy initialisation / expansion */
    ensure_limit(n);

    /* n should now be within the computed range */
    return perm[n];
}
