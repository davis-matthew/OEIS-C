/*
 * A369640  a(n) = 1 if n is composite and n' is a sum of distinct primorial
 *          numbers, otherwise 0, where n' stands for the arithmetic
 *          derivative of n, A003415.
 * https://oeis.org/A369640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is composite and its arithmetic derivative n' is a
 *            sum of distinct primorial numbers, otherwise 0
 * Range:     u8 n <= 7, u16 n <= 8946, u32 n <= 8946, u64 n <= 8946,
 *            u128 n <= 8946, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

#define MAX_PRIMORIALS 7  /* 0# , 2 , 6 , 30 , 210 , 2310 , 30030  */

static seq_t primorials[MAX_PRIMORIALS];
static seq_t subset_sums[1U << MAX_PRIMORIALS];
static bool primorials_initialized = false;

static void init_primorials(void)
{
    if (primorials_initialized)
        return;

    /* 0# = 1 */
    primorials[0] = 1;
    seq_t prod = 1;
    const uint64_t primes_for_primorials[] = { 2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL };
    for (size_t i = 0; i < sizeof(primes_for_primorials)/sizeof(primes_for_primorials[0]); ++i) {
        prod *= (seq_t)primes_for_primorials[i];
        primorials[i + 1] = prod;          /* 2..6th primorial */
    }

    /* Pre‑compute all subset sums of the primorials.  There are 2^7 = 128 subsets. */
    size_t total_masks = 1U << MAX_PRIMORIALS;
    for (size_t mask = 0; mask < total_masks; ++mask) {
        seq_t sum = 0;
        for (size_t b = 0; b < MAX_PRIMORIALS; ++b) {
            if (mask & (1U << b))
                sum += primorials[b];
        }
        subset_sums[mask] = sum;
    }

    primorials_initialized = true;
}

seq_t a(idx_t n)
{
    /* 0, 1, 2, 3 are not composite. */
    if (n < 4)
        return 0;

    /* Quick composite check. */
    if (is_prime(n))
        return 0;

    init_primorials();

    /* Compute arithmetic derivative n' = Σ e_i * n/p_i. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t derivative = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = ((seq_t)(n / p)) * (seq_t)e;
        derivative += term;
    }

    /* Check if derivative equals any subset sum of distinct primorials. */
    size_t total_masks = 1U << MAX_PRIMORIALS;
    for (size_t mask = 0; mask < total_masks; ++mask) {
        if (subset_sums[mask] == derivative)
            return 1;
    }
    return 0;
}
