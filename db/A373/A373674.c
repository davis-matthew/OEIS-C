/*
 * A373674  Last element of each maximal run of powers of primes (including
 *          1).
 * https://oeis.org/A373674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   seq_t_compare
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373674: a(n) = last element of each maximal run of powers of primes (including 1) */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_t_compare.h"

/* Simple sieve – returns array of primes ≤ limit, and the count */
static void generate_primes_upto(size_t limit, uint64_t **out, size_t *cnt)
{
    bool *is_composite = (bool *)calloc(limit + 1, 1);
    if (!is_composite) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
            count++;
        }
    }
    uint64_t *primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i])
            primes[idx++] = (uint64_t)i;
    }
    free(is_composite);
    *out = primes;
    *cnt = count;
}

/* Static data for memoisation */
static seq_t *runs = NULL;          /* array of run ends */
static size_t runs_len = 0;         /* number of runs stored */
static size_t current_limit = 0;    /* last limit used in regeneration */

/* Recompute the run array until at least 'needed' elements are present */
static void recompute_runs(size_t needed)
{
    while (runs_len < needed) {
        size_t new_limit = (current_limit == 0) ? 1000 : current_limit * 2;

        /* Generate primes up to new_limit */
        uint64_t *primes = NULL;
        size_t primes_cnt = 0;
        generate_primes_upto(new_limit, &primes, &primes_cnt);

        /* Build array of all prime powers ≤ new_limit (including 1) */
        size_t pow_cap = primes_cnt * 5 + 1;
        seq_t *powers = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        size_t pws = 0;
        powers[pws++] = (seq_t)1;              /* include 1 */
        seq_t lim_seq = (seq_t)new_limit;

        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            seq_t base = (seq_t)primes[pi];
            seq_t pp   = base;
            while (pp <= lim_seq) {
                if (pws >= pow_cap) {
                    pow_cap <<= 1;               /* double capacity */
                    powers = (seq_t *)realloc(powers, pow_cap * sizeof(seq_t));
                }
                powers[pws++] = pp;
                if (pp > lim_seq / base) break;   /* would overflow / exceed limit */
                pp *= base;
            }
        }

        /* Sort the powers (they are not globally sorted yet) */
        qsort(powers, pws, sizeof(seq_t), seq_t_compare);

        /* Compute run ends: last element of each maximal run */
        free(runs);                             /* free old run list if any */
        runs = (seq_t *)malloc((pws + 1) * sizeof(seq_t));
        runs_len = 0;
        for (size_t i = 0; i < pws; ++i) {
            if (i > 0 && powers[i] != powers[i - 1] + (seq_t)1)
                runs[runs_len++] = powers[i - 1];
        }
        /* Add the final run's last element */
        if (pws > 0)
            runs[runs_len++] = powers[pws - 1];

        /* Clean up */
        free(powers);
        free(primes);
        current_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* out of range → 0 */
    recompute_runs((size_t)n);
    return runs[(size_t)n - 1];
}
