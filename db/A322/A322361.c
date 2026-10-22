/*
 * A322361  a(n) = gcd(n, A003961(n)), where A003961 is completely
 *          multiplicative with a(prime(k)) = prime(k+1).
 * https://oeis.org/A322361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A003961(n))
 * Range:     u8 n <= 1154, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 is completely multiplicative with A003961(p_k) = p_{k+1}. */
/* The function computes the gcd by factoring n and using the fact that */
/* the multiplicative image shifts each prime factor to the next prime. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Return a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* Lazily build a prime list that covers all primes needed for n ≤ 10000 */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt   = 0;
    static int once = 0;
    if (!once) {
        idx_t limit = 20000;                     /* > max prime needed */
        build_primes_upto(limit, &prime_list, &prime_cnt);
        once = 1;
    }

    /* Factor n using the pre‑computed prime list */
    uint64_t primes_f[64];   /* primes that divide n */
    uint64_t exps_f[64];     /* corresponding exponents */
    size_t cf = 0;           /* number of distinct prime factors */

    uint64_t tmp = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes_f[cf] = p;
            exps_f[cf]   = e;
            ++cf;
        }
    }
    if (tmp > 1) { /* remaining prime factor > sqrt(n) */
        primes_f[cf] = tmp;
        exps_f[cf]   = 1;
        ++cf;
    }

    /* Compute the gcd(n, A003961(n)) */
    seq_t result = 1;

    for (size_t i = 0; i < cf; ++i) {
        uint64_t p    = primes_f[i];
        uint64_t exp_p = exps_f[i];

        /* Find the index of p in the global prime list */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo >= prime_cnt || prime_list[lo] != p)
            continue;          /* should not happen */
        if (lo == 0)
            continue;          /* p has no previous prime, contributes nothing */

        uint64_t prev_p = prime_list[lo - 1];

        /* Find exp of prev_p in the factorization of n */
        size_t j;
        for (j = 0; j < cf; ++j) {
            if (primes_f[j] == prev_p)
                break;
        }
        if (j == cf)
            continue;          /* prev_p not a factor of n */

        uint64_t exp_prev = exps_f[j];

        uint64_t exp = (exp_p < exp_prev) ? exp_p : exp_prev;
        for (uint64_t k = 0; k < exp; ++k)
            result *= (seq_t)p;
    }

    return result;
}
