/*
 * A318979  Number of divisors of n with relatively prime prime indices,
 *          meaning they belong to A289509.
 * https://oeis.org/A318979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of divisors of n whose prime indices have greatest
 *            common divisor 1 (excluding divisor 1)
 * Range:     u8 n <= 9757, u16 n <= 9757, u32 n <= 9757, u64 n <= 9757,
 *            u128 n <= 9757, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Prime cache (1‑based indices). */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t     prime_cnt  = 0;     /* number of primes in array */
static idx_t      prime_limit = 0;    /* highest prime currently stored */

/* Return the 1‑based index of prime p. Builds cache if necessary. */
static idx_t get_prime_index(uint64_t p)
{
    if (p > prime_limit) {
        uint64_t *new_list;
        size_t    new_cnt;
        build_primes_upto(p, &new_list, &new_cnt);
        free(prime_list);
        prime_list = new_list;
        prime_cnt  = new_cnt;
        prime_limit = p;
    }

    /* Binary search. */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }

    if (lo < prime_cnt && prime_list[lo] == p)
        return (idx_t)(lo + 1);   /* indices are 1‑based */
    return (idx_t)0;              /* should not happen */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;   /* a(1) = 0 */

    /* Factor n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   k = 0;
    factor_u64((uint64_t)n, primes, exps, &k);
    if (k == 0)
        return 0;   /* should not happen for n>=2 */

    /* Indices of the prime factors. */
    idx_t idxs[64];
    for (size_t i = 0; i < k; ++i)
        idxs[i] = get_prime_index(primes[i]);

    seq_t total = 0;
    size_t total_masks = (size_t)1 << k;   /* k < 64 for n <= 10^4 */

    for (size_t mask = 1; mask < total_masks; ++mask) {
        /* Compute gcd of indices in this subset. */
        idx_t gcd_all = 0;
        seq_t prod = 1;

        for (size_t i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i)) {
                /* Update product of exponent choices. */
                prod *= (seq_t)exps[i];

                /* Update gcd of indices. */
                if (gcd_all == 0)
                    gcd_all = idxs[i];
                else {
                    idx_t a = gcd_all, b = idxs[i];
                    while (b) { idx_t t = a % b; a = b; b = t; }
                    gcd_all = a;
                }
            }
        }

        if (gcd_all == 1)   /* divisor valid (excludes the divisor 1) */
            total += prod;
    }

    return total;
}
