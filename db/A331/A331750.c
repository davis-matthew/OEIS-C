/*
 * A331750  a(n) = A048675(sigma(n)).
 * https://oeis.org/A331750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048675(sigma(n))
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 276, u64 n <= 660,
 *            u128 n <= 728, big any n
 * Helpers:   build_primes_upto, factor_u64, mul_small, pow2, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A331750: a(n) = A048675(σ(n))
 * where A048675(n) = Σ_{p^e‖n} e·2^rank(p),
 * rank(p) being the zero‑based index of prime p in the increasing list of primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/mul_small.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* list of primes */
static size_t prime_count = 0;        /* number of primes in prime_list */
static idx_t primes_limit = 0;        /* maximum prime value in prime_list */

/* Ensure prime_list contains all primes ≤ limit.  Reallocate if needed. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;

    /* Free any old allocation before reallocating */
    if (prime_list) free(prime_list);

    build_primes_upto(limit,
                       (uint64_t **)&prime_list,
                       &prime_count);
    primes_limit = limit;
}

/* Binary search for the 0‑based index of prime p in prime_list. */
static int prime_rank(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (int)mid;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1; /* should not happen for a proper prime factor */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; sequence starts at index 1 */

    /* Compute σ(n). */
    seq_t sigma = sigma_of(n);

    /* σ(1) = 1 gives a(1) = 0. */
    if (sigma <= 1) return 0;

    /* Convert σ(n) to uint64_t for factorization; assumes it fits. */
    uint64_t sigma_u = (uint64_t)sigma;

    /* Ensure we have all primes up to σ(n). */
    ensure_primes((idx_t)sigma_u);

    /* Factor σ(n). */
    uint64_t fac[64];
    uint64_t exp[64];
    size_t fac_cnt = 0;
    factor_u64(sigma_u, fac, exp, &fac_cnt);

    seq_t result = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac[i];
        uint64_t e = exp[i];
        int r = prime_rank(p);
        if (r < 0) continue;       /* safety – should not happen */
        seq_t term = mul_small(pow2((idx_t)r), (unsigned int)e);
        result += term;
    }
    return result;
}
