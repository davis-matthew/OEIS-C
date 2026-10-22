/*
 * A316437  Take the integer partition with Heinz number n, divide all parts
 *          by the GCD of the parts, then take the Heinz number of the
 *          resulting partition.
 * https://oeis.org/A316437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ p⁽ʷ⁽p⁾⁾ prime(idx(p)/g) ˢ; where g = gcd{idx(p)} over
 *            all prime divisors p of n, idx(p) is the index of prime p, w(p)
 *            the exponent of p in n, and “∏ p⁽ʷ⁽p⁾⁾” denotes the product over
 *            all prime factors of n. This implements the definition: take the
 *            partition whose Heinz number is n, divid...
 * Range:     u8 n <= 255, u16 n <= 9879, u32 n <= 9879, u64 n <= 9879,
 *            u128 n <= 9879, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Small internal GCD for 64‑bit unsigned integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Global storage for the list of primes */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;

/* Initialise (once) the prime list up to a fixed generous limit.
   2 000 000 contains 148 933 primes, more than enough for n up to ~10⁶. */
static void init_prime_list(void)
{
    if (prime_list != NULL) return;
    /* build_primes_upto allocates memory; it will be freed on program exit. */
    build_primes_upto(2000000, &prime_list, &prime_cnt);
}

/* Binary search for the 1‑based index of a prime number p in prime_list. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else                      hi = mid;
    }
    /* p is guaranteed to be prime and present in the list. */
    return lo + 1;           /* 1‑based index */
}

/* The core sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;       /* a(1)=1 */

    /* Factorise n. 64‑bit cache is sufficient for factorisation of n. */
    uint64_t fac_primes[64];
    uint64_t fac_exps  [64];
    size_t   fac_cnt   = 0;

    factor_u64((uint64_t)n, fac_primes, fac_exps, &fac_cnt);

    /* Ensure we have the primes table loaded once. */
    init_prime_list();

    /* Compute the GCD of all prime indices in the factorisation. */
    uint64_t g = 0;
    size_t   idx[64];
    for (size_t i = 0; i < fac_cnt; ++i) {
        idx[i] = prime_index(fac_primes[i]);
        g = (g == 0) ? idx[i] : gcd_uint64(g, idx[i]);
    }

    /* Accumulate the result. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        /* The new index after division by g. */
        uint64_t new_idx = idx[i] / g;
        /* new_idx is at least 1 and within the pre‑computed list. */
        seq_t base = (seq_t)prime_list[new_idx - 1];   /* prime at that index */

        /* Multiply base^exp into the result. */
        seq_t term = (seq_t)1;
        for (uint64_t k = 0; k < fac_exps[i]; ++k) {
            term *= base;
        }
        result *= term;
    }

    return result;
}
