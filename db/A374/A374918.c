/*
 * A374918  Even numbers k such that lambda(sum of even divisors of k) =
 *          phi(sum of odd divisors of k) where lambda is the Carmichael
 *          function (A002322) and phi the Euler totient function (A000010).
 * https://oeis.org/A374918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth even number k such that λ(σ_even(k)) =
 *            φ(σ_odd(k)). The function searches k incrementally, caching the
 *            found terms
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   calc_phi, factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"         /* gcd(seq_t a, seq_t b) */
#include "../utils/calc_phi.h"    /* seq_t calc_phi(seq_t m) */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

static seq_t lambda_of(seq_t n)
{
    if (n <= 1) return 1;          /* λ(1) = 1, λ(0) not used */
    uint64_t m = (uint64_t)n;      /* n fits in 64‑bits for this sequence */
    seq_t l = 1;

    /* Factor 2 separately */
    int e2 = 0;
    while ((m & 1ULL) == 0) { m >>= 1; e2++; }
    if (e2) {
        seq_t lambda2;
        if (e2 == 1)                lambda2 = 1;
        else if (e2 == 2)           lambda2 = 2;
        else                        lambda2 = pow_uint(2, e2 - 2);
        seq_t g = gcd(l, lambda2);
        l = l / g * lambda2;
    }

    /* Factor odd primes */
    uint64_t prime, exp;
    size_t cnt = 0;
    factor_u64(m, &prime, &exp, &cnt);  /* factor_u64 fills arrays with primes and exps */
    /* Since factor_u64 only provides one factor at a time, loop manually */
    uint64_t tmp = m;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pp = 0;
    factor_u64(tmp, primes, exps, &pp); /* factor_u64 should work like this */
    for (size_t i = 0; i < pp; ++i) {
        prime = primes[i];
        exp   = exps[i];
        seq_t pow_pe = pow_uint(prime, exp);            /* prime^exp */
        seq_t lambda_p = calc_phi(pow_pe);              /* λ(prime^exp) = φ(prime^exp) */
        seq_t g = gcd(l, lambda_p);
        l = l / g * lambda_p;
    }

    return l;
}

static void ensure_capacity(size_t needed, seq_t **cache, size_t *cap)
{
    if (*cap >= needed) return;
    size_t newcap = *cap ? *cap * 2 : 128;
    while (newcap < needed) newcap <<= 1;
    *cache = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
    *cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 2;    /* smallest even candidate */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k;
        uint64_t tmp = k;
        int e2 = 0;
        while ((tmp & 1ULL) == 0) { tmp >>= 1; e2++; }
        uint64_t odd_part = tmp;        /* m */

        /* Compute σ(odd_part) */
        seq_t sigma_odd = 1;
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(odd_part, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            seq_t term = (pow_uint(p, e + 1) - 1) / ((seq_t)p - 1);
            sigma_odd *= term;
        }

        /* Sum of odd and even divisors */
        seq_t sum_odd  = sigma_odd;
        seq_t sum_even = sigma_odd * (pow_uint(2, e2 + 1) - 2);

        seq_t lambda_even = lambda_of(sum_even);
        seq_t phi_odd     = calc_phi(sum_odd);

        if (lambda_even == phi_odd) {
            ensure_capacity(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)k;
        }

        next_k += 2;   /* move to next even number */
    }
    return cache[n - 1];
}
