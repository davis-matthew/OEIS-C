/*
 * A336122  Numbers k for which A335884(k) = 2.
 * https://oeis.org/A336122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   odd_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A336122
 * Numbers k such that, for every choice of an odd prime factor p of k
 * and either sign ±, the number m = k ± k/p is a power of two or
 * every number obtained from m by the same operation is a power of two.
 * Moreover, at least one first step must produce a non‑power of two,
 * i.e. there is a branch that needs exactly two steps to reach a power of two.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_prime_factors.h"

/* return true if x is a power of two */
static bool is_power_of_two(uint64_t x)
{
    return x && !(x & (x - 1));
}

/* true if k belongs to A336122 */
static bool is_in_seq(uint64_t k)
{
    if (k <= 1)
        return false;

    uint64_t pk[20];
    size_t ck;
    odd_prime_factors(k, pk, &ck);
    if (ck == 0)                   /* k is a power of two */
        return false;

    bool has_non_power_branch = false;

    for (size_t i = 0; i < ck; ++i) {
        uint64_t p = pk[i];
        uint64_t step = k / p;

        /* k - step */
        uint64_t m = k - step;
        if (!is_power_of_two(m)) {
            has_non_power_branch = true;
            uint64_t qm[20];
            size_t cm;
            odd_prime_factors(m, qm, &cm);
            if (cm == 0)
                return false;        /* should not happen */
            for (size_t j = 0; j < cm; ++j) {
                uint64_t q = qm[j];
                uint64_t step2 = m / q;
                uint64_t m2 = m - step2;
                if (!is_power_of_two(m2))
                    return false;
                m2 = m + step2;
                if (!is_power_of_two(m2))
                    return false;
            }
        }

        /* k + step */
        m = k + step;
        if (!is_power_of_two(m)) {
            has_non_power_branch = true;
            uint64_t qm[20];
            size_t cm;
            odd_prime_factors(m, qm, &cm);
            if (cm == 0)
                return false;        /* should not happen */
            for (size_t j = 0; j < cm; ++j) {
                uint64_t q = qm[j];
                uint64_t step2 = m / q;
                uint64_t m2 = m - step2;
                if (!is_power_of_two(m2))
                    return false;
                m2 = m + step2;
                if (!is_power_of_two(m2))
                    return false;
            }
        }
    }

    return has_non_power_branch;
}

/* static cache */
static uint64_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, but guard */

    /* ensure cache capacity */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
    }
    if (cache_size < (size_t)n) {
        /* grow cache if needed */
        if (n > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)n)
                new_cap <<= 1;
            cache = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
            cache_cap = new_cap;
        }
        /* fill cache until we have n elements */
        while (cache_size < (size_t)n) {
            while (!is_in_seq(next_candidate))
                ++next_candidate;
            cache[cache_size++] = next_candidate;
            ++next_candidate;
        }
    }

    return (seq_t)cache[n - 1];
}
