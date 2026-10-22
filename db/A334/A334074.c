/*
 * A334074  a(n) is the numerator of the sum of reciprocals of primes not
 *          exceeding n and not dividing binomial(2*n, n).
 * https://oeis.org/A334074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( Σ_{p prime ≤ n, binom(2n,n) % p > 0} 1/p )
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 86, u128 n <= 175,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: for each n, find the primes p ≤ n that do NOT divide
 * C(2n,n).  Let S be that set.  If S = {p₁,…,p_k}, then
 *   denom = p₁·p₂·…·p_k
 *   numer = Σ denom/pᵢ
 *   a(n) = numer / gcd(numer, denom)
 * (return 0 if S is empty).  This uses the helper functions
 * build_primes_upto and gcd supplied by the build.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

/* Global storage for the primes list */
static uint64_t *primes_list = NULL;
static size_t   primes_cnt  = 0;
static idx_t    primes_lim  = 0;

/* Temporary buffer for primes that remain in the sum */
static uint64_t *remain_buf = NULL;
static size_t   remain_cap  = 0;

/* ----------  Helper ------------------------------------------------- */

/* exponent of prime p in n! (Legendre's formula) */
static inline idx_t vp_factorial(idx_t n, uint64_t p) {
    idx_t exp = 0;
    idx_t div = p;
    while (div <= n) {
        exp += n / div;
        if (div > n / p) break;             /* avoid overflow */
        div *= p;
    }
    return exp;
}

/* ----------  Main function ------------------------------------------ */

seq_t a(idx_t n) {
    if (n < 1) return 0;                     /* safety for misuse */

    /* Grow prime list if needed */
    if (n > primes_lim) {
        free(primes_list);
        build_primes_upto(n, &primes_list, &primes_cnt);
        primes_lim = n;
    }

    /* Allocate temporary buffer for the primes in S */
    if (remain_cap < primes_cnt) {
        free(remain_buf);
        remain_buf = (uint64_t*)calloc(primes_cnt, sizeof(uint64_t));
        remain_cap = primes_cnt;
    }
    size_t remain_cnt = 0;

    /* Determine the set S: primes ≤ n that do NOT divide C(2n,n) */
    idx_t two_n = n * 2ULL;
    for (size_t idx = 0; idx < primes_cnt && primes_list[idx] <= n; ++idx) {
        uint64_t p = primes_list[idx];
        idx_t e2 = vp_factorial(two_n, p);
        idx_t e1 = vp_factorial(n, p);
        if (e2 > 2 * e1) {            /* p divides the binomial coefficient */
            continue;
        }
        remain_buf[remain_cnt++] = p;
    }

    /* No primes in the sum → a(n)=0 */
    if (remain_cnt == 0) return 0;

    /* Compute denominator = product of all primes in S */
    seq_t denom = 1;
    for (size_t i = 0; i < remain_cnt; ++i) {
        denom *= (seq_t)remain_buf[i];
    }

    /* Compute numerator = Σ denom/p */
    seq_t numer = 0;
    for (size_t i = 0; i < remain_cnt; ++i) {
        numer += denom / (seq_t)remain_buf[i];
    }

    /* Reduce fraction */
    seq_t g = gcd(numer, denom);
    numer /= g;
    return numer;
}
