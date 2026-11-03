/*
 * A332534  Numbers that are not of the form prime + 2^(2^k) + m! with k >= 0,
 *          m >= 0.
 * https://oeis.org/A332534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer NOT expressible as prime + 2^(2^k) + m! for
 *            k>=0, m>=0
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9241 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to allocate an array of seq_t of given size */
static seq_t *alloc_seqarray(size_t n)
{
    return n ? mem_new(seq_t, n) : NULL;
}

/* Compute the list of complementary numbers up to a given limit.
   This function is called only once (or when more numbers are needed). */
static void compute_complement(idx_t limit,
                               seq_t **out, size_t *out_len,
                               idx_t *exceeded_limit)
{
    /* Build list of factorials <= limit */
    uint64_t fact = 1;
    size_t fact_cnt = 0;
    uint64_t *facts = NULL;

    while (fact <= (uint64_t)limit) {
        facts = (uint64_t *)realloc(facts, sizeof(uint64_t) * (fact_cnt + 1));
        facts[fact_cnt++] = fact;
        if (fact > (uint64_t)limit / (fact_cnt + 1)) break; /* prevent overflow */
        fact *= (fact_cnt + 1); /* next factorial */
    }

    /* Build list of powers 2^(2^k) <= limit */
    uint64_t power = 2;
    size_t power_cnt = 0;
    uint64_t *powers = NULL;
    while (power <= (uint64_t)limit) {
        powers = (uint64_t *)realloc(powers, sizeof(uint64_t) * (power_cnt + 1));
        powers[power_cnt++] = power;
        if (power > (uint64_t)limit / power) break;
        power = power * power; /* next 2^(2^k) */
    }

    /* Sieve primes up to limit */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, 1);
    if (!is_comp) exit(1);

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Collect primes in an array */
    size_t primes_cnt = 0;
    uint32_t *primes = NULL;
    for (idx_t i = 2; i <= limit; ++i) if (!is_comp[i]) {
        primes = (uint32_t *)realloc(primes, sizeof(uint32_t) * (primes_cnt + 1));
        primes[primes_cnt++] = (uint32_t)i;
    }

    /* Mark numbers that are representable as p + 2^(2^k) + m! */
    bool *inS = (bool *)calloc((size_t)limit + 1, 1);
    if (!inS) exit(1);

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        for (size_t piw = 0; piw < power_cnt; ++piw) {
            uint64_t pow2 = powers[piw];
            for (size_t pif = 0; pif < fact_cnt; ++pif) {
                uint64_t sum = (uint64_t)p + pow2 + facts[pif];
                if (sum <= (uint64_t)limit)
                    inS[(size_t)sum] = true;
                else
                    break;
            }
        }
    }

    /* Count complement numbers */
    size_t comp_cnt = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (!inS[(size_t)i]) ++comp_cnt;

    *out = mem_new(seq_t, comp_cnt);
    if (!(*out)) exit(1);
    *out_len = comp_cnt;

    /* Fill complement list */
    size_t idx = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (!inS[(size_t)i]) (*out)[idx++] = (seq_t)i;

    /* Record if limit was exceeded in generating sums */
    if (exceeded_limit) *exceeded_limit = limit;

    /* Free temporary arrays */
    free(is_comp);
    free(inS);
    free(primes);
    free(facts);
    free(powers);
}

/* Static cache variables */
static seq_t *comps = NULL;
static size_t comps_len = 0;
static idx_t current_limit = 0;

/* Ensure that comps contains at least n terms */
static void ensure_terms(idx_t n)
{
    if (n <= comps_len) return;

    /* Start with a reasonable limit and double until enough terms */
    idx_t lim = current_limit;
    if (lim == 0) lim = 1000000; /* initial guess */
    while (true) {
        compute_complement(lim, &comps, &comps_len, NULL);
        if (n <= comps_len) break;
        /* Not enough, double the limit and recompute */
        idx_t new_lim = lim * 2;
        mem_free(comps); comps_len = 0;
        lim = new_lim;
    }
    current_limit = lim;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_terms(n);
    return comps[n - 1];
}
