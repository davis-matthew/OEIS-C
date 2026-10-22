/*
 * A354103  a(n) = sigma(A267099(n)), where A267099 is fully multiplicative
 *          involution swapping the positions of 4k+1 and 4k+3 primes, and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A354103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A267099(n))
 * Range:     u8 n <= 10, u16 n <= 556, u32 n <= 9383, u64 n <= 9383,
 *            u128 n <= 9383, big any n
 * Helpers:   build_primes, build_spf, seq_pow_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A267099 is the fully multiplicative involution swapping 4k+1 and 4k+3 primes,
   fixing 2.  The function computes the sum of divisors of the image of n
   under that involution. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes.h"
#include "../utils/seq_pow_simple.h"

static bool initialized = false;
static idx_t *spf = NULL;          /* smallest prime factor array up to LIMIT */
static uint64_t *primes1 = NULL;   /* primes ≡ 1 (mod 4) */
static size_t cnt1 = 0;
static uint64_t *primes3 = NULL;   /* primes ≡ 3 (mod 4) */
static size_t cnt3 = 0;

static void init(void) {
    if (initialized)
        return;

    /* Sufficiently large limit to cover primes needed for n <= 10000.
       The mapping only depends on the order of primes of each residue class,
       so 50000 is safe. */
    const idx_t LIMIT = 50000;

    /* Smallest‑prime‑factor sieve. */
    build_spf(LIMIT, &spf);

    /* List all primes up to LIMIT.  build_primes returns idx_t*, which is
       the same underlying type as uint64_t. */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(LIMIT, &primes, &primes_cnt);

    /* Count primes ≡ 1 and 3 modulo 4 (excluding 2). */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p == 2)
            continue;
        if (p % 4 == 1)
            ++cnt1;
        else /* p % 4 == 3 */
            ++cnt3;
    }

    /* Allocate arrays for the two residue classes. */
    primes1 = (uint64_t*)malloc(cnt1 * sizeof(uint64_t));
    primes3 = (uint64_t*)malloc(cnt3 * sizeof(uint64_t));

    /* Fill the residue arrays. */
    size_t i1 = 0, i3 = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p == 2)
            continue;
        if (p % 4 == 1)
            primes1[i1++] = p;
        else
            primes3[i3++] = p;
    }

    /* Free the temporary primes list. */
    free(primes);

    initialized = true;
}

/* Map a prime p under the involution.  For 2 it returns 2. */
static uint64_t map_prime(uint64_t p) {
    if (p == 2)
        return 2;

    if (p % 4 == 1) {            /* map to corresponding 3 mod 4 prime */
        for (size_t i = 0; i < cnt1; ++i)
            if (primes1[i] == p)
                return primes3[i];
    } else {                      /* p % 4 == 3, map to 1 mod 4 prime */
        for (size_t i = 0; i < cnt3; ++i)
            if (primes3[i] == p)
                return primes1[i];
    }
    /* Should never reach here if p is prime and >2. */
    return p;
}

seq_t a(idx_t n) {
    init();

    idx_t k = n;               /* working copy */
    seq_t result = (seq_t)1;

    while (k > 1) {
        idx_t p = spf[k];
        idx_t e = 0;
        while ((k % p) == 0) {
            k /= p;
            ++e;
        }

        uint64_t mapped = map_prime((uint64_t)p);
        seq_t base = (seq_t)mapped;

        /* Compute (base^(e+1)-1)/(base-1) */
        seq_t pow_val = seq_pow_simple(base, (uint64_t)(e + 1));
        seq_t numerator   = pow_val - (seq_t)1;
        seq_t denominator = base - (seq_t)1;
        seq_t factor = numerator / denominator;   /* integer division */

        result *= factor;
    }

    return result;
}
