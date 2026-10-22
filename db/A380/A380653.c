/*
 * A380653  Number of positive integers less than or equal to n that have the
 *          same sum of prime factors (with repetition) as n.
 * https://oeis.org/A380653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{j <= n : sopfr(j) = sopfr(n)}|
 * Range:     u8 n <= 9697, u16 n <= 9697, u32 n <= 9697, u64 n <= 9697,
 *            u128 n <= 9697, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute sum of prime factors with multiplicity for an integer up to n.
   The sums are stored in a static array for quick reuse. */
static uint64_t *sum_pf = NULL;
static uint64_t *spf = NULL;
static idx_t max_precomp = 0;

/* Helper: sieve to fill smallest prime factor array up to limit. */
static void init_spf(idx_t limit)
{
    if (limit <= max_precomp && spf != NULL)
        return;                    /* already ready */

    /* allocate or reallocate spf array */
    uint64_t *new_spf = (uint64_t *)malloc((limit + 1) * sizeof(*new_spf));
    if (!new_spf) abort();

    for (idx_t i = 2; i <= limit; ++i)
        new_spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = i;
            }
        }
    }

    /* free old array */
    free(spf);
    spf = new_spf;

    /* If sum_pf was allocated earlier, enlarge it to new limit */
    if (sum_pf != NULL && max_precomp < limit) {
        uint64_t *new_sum = (uint64_t *)malloc((limit + 1) * sizeof(*new_sum));
        if (!new_sum) abort();
        for (idx_t i = 0; i <= max_precomp; ++i)
            new_sum[i] = sum_pf[i];
        free(sum_pf);
        sum_pf = new_sum;
    } else if (sum_pf == NULL) {         /* first allocation */
        sum_pf = (uint64_t *)malloc((limit + 1) * sizeof(*sum_pf));
        if (!sum_pf) abort();
    }

    /* precompute sums for new indices */
    for (idx_t i = 1; i <= limit; ++i) {
        if (i == 1) {
            sum_pf[i] = 0;          /* by definition sopfr(1) = 0 */
        } else if (i <= max_precomp) {
            /* already computed, skip */
        } else {
            uint64_t x = i;
            uint64_t s = 0;
            while (x > 1) {
                uint64_t p = spf[x];
                s += p;
                x /= p;
            }
            sum_pf[i] = s;
        }
    }

    max_precomp = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but return 0 */

    /* Expand precomputed data if needed */
    if (n > max_precomp)
        init_spf(n);

    /* Get the target sum of prime factors for n */
    uint64_t target_sum = sum_pf[n];

    /* Count occurrences up to n with same sum */
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (sum_pf[i] == target_sum) {
            count += 1;
        }
    }

    return count;
}
