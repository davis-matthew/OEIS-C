/*
 * A248692  Fully multiplicative with a(prime(i)) = 2^i; If n = Product_{k >=
 *          1} (p_k)^(c_k) where p_k is k-th prime A000040(k) and c_k >= 0
 *          then a(n) = Product_{k >= 1} 2^(k*c_k).
 * https://oeis.org/A248692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^c∥n} 2^{index(p)·c} where index(p) is the 1‑based
 *            position of p in the prime listing 2,3,5,7,… (index(2)=1,
 *            index(3)=2, …)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

/* Global cache of primes up to the largest limit requested. */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;
static idx_t     prime_limit = 0;

/* Ensure that primes up to 'limit' are available. */
static void ensure_primes(idx_t limit)
{
    if (prime_list && limit <= prime_limit)
        return;

    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
    }
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Return the 1‑based prime index of prime p (or 0 if p is not prime). */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_count && prime_list[lo] == p) ? (idx_t)(lo + 1) : 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return pow2(0);   /* a(1) = 1 */

    /* Prepare primes up to at least n. */
    ensure_primes(n);

    uint64_t temp = (uint64_t)n;
    idx_t    exp_sum = 0;

    while (temp > 1) {
        uint64_t p = 0;
        /* Find smallest prime factor using the cached prime list. */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t pr = prime_list[i];
            if (pr > temp / pr) /* pr > sqrt(temp) */
                break;
            if (temp % pr == 0) {
                p = pr;
                break;
            }
        }
        if (p == 0)          /* temp itself is prime */
            p = temp;

        /* Count multiplicity of p. */
        uint64_t c = 0;
        while (temp % p == 0) {
            temp /= p;
            ++c;
        }

        /* Accumulate weighted exponent. */
        exp_sum += (idx_t)prime_index(p) * c;
    }

    return pow2(exp_sum);
}
