/*
 * A376120  Refactorable numbers that are perfect powers.
 * https://oeis.org/A376120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   mul_overflow_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Refactorable perfect powers (OEIS A376120) – intersection of A001597 and A033950 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mul_overflow_check.h"

/* Compute base^exp with an upper bound limit.  Returns false if the result would
   exceed limit. */
static bool pow_with_limit(uint64_t base, uint64_t exp, uint64_t limit, uint64_t *res)
{
    uint64_t r = 1;
    for (uint64_t i = 0; i < exp; ++i)
        if (!mul_overflow_check(r, base, limit, &r))
            return false;
    *res = r;
    return true;
}

/* Sieve primes up to sqrt(limit).  The resulting primes are stored in
   global array primes[]; primes_cnt holds the count. */
static uint32_t primes[65000];
static size_t primes_cnt = 0;

static void sieve_primes(void)
{
    const uint32_t max_sieve = 65000;      /* > sqrt(4e9) */
    bool comp[max_sieve + 1];
    memset(comp, 0, sizeof(comp));

    primes_cnt = 0;
    for (uint32_t p = 2; p <= max_sieve; ++p) {
        if (!comp[p]) {
            primes[primes_cnt++] = p;
            if ((uint64_t)p * p <= max_sieve) {
                for (uint32_t j = p * p; j <= max_sieve; j += p)
                    comp[j] = true;
            }
        }
    }
}

/* Compute divisor count for n (n <= current limit). */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t res = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            unsigned e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            res *= (e + 1);
        }
    }
    if (tmp > 1)
        res *= 2;
    return res;
}

/* Compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t da = *(const uint64_t *)a;
    uint64_t db = *(const uint64_t *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

/* Storage for sequence cache */
#define CACHE_CAP 200000     /* enough for 10000 terms */
static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;

/* Temporary storage for powers */
#define POW_CAP 200000
static uint64_t raw_powers[POW_CAP];
static uint64_t unique_powers[POW_CAP];

/* Current upper bound for power generation */
static uint64_t power_limit = 4000000000ULL;   /* 4e9 */

/* Rebuild everything for the current power_limit */
static void rebuild(void)
{
    /* generate primes once */
    sieve_primes();

    /* generate raw perfect powers */
    size_t raw_cnt = 0;
    raw_powers[raw_cnt++] = 1;   /* 1 is a perfect power */

    /* compute maximum exponent so that 2^exponent <= power_limit */
    uint64_t max_exp = 2;
    while (true) {
        uint64_t val;
        if (!pow_with_limit(2, max_exp, power_limit + 1, &val))
            break;
        ++max_exp;
    }

    for (uint64_t exp = 2; exp < max_exp; ++exp) {
        for (uint64_t base = 2; ; ++base) {
            uint64_t val;
            if (!pow_with_limit(base, exp, power_limit, &val))
                break;
            if (raw_cnt >= POW_CAP) {
                /* should not happen for our limits */
                break;
            }
            raw_powers[raw_cnt++] = val;
        }
    }

    /* sort and deduplicate */
    qsort(raw_powers, raw_cnt, sizeof(uint64_t), cmp_uint64);

    size_t uniq_cnt = 0;
    for (size_t i = 0; i < raw_cnt; ++i) {
        if (i == 0 || raw_powers[i] != raw_powers[i - 1]) {
            unique_powers[uniq_cnt++] = raw_powers[i];
        }
    }

    /* build the filtered sequence */
    cache_size = 0;
    for (size_t i = 0; i < uniq_cnt; ++i) {
        uint64_t val = unique_powers[i];
        uint64_t tau = divisor_count(val);
        if (val % tau == 0) {
            if (cache_size < CACHE_CAP)
                cache[cache_size++] = (seq_t)val;
        }
    }
}

/* Ensure that the cache holds at least 'n' terms.  If needed, double the
   power_limit and rebuild. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;
    while (cache_size < (size_t)n) {
        /* double the limit; guard against overflow */
        if (power_limit > UINT64_MAX / 2) {
            /* cannot extend further – break to avoid infinite loop */
            break;
        }
        power_limit *= 2;
        rebuild();
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        rebuild();
        initialized = true;
    }
    ensure_cache(n);
    if (n == 0 || (size_t)n > cache_size)
        return (seq_t)0;   /* value for indices below offset or out of range */
    return cache[(size_t)n - 1];
}
