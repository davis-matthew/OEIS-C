/*
 * A318809  Number of necklace permutations of the multiset of prime indices
 *          of n > 1.
 * https://oeis.org/A318809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/L) * Σ_{d|L, h|m_j} φ(L/d) * d! / ∏ (m_j/h)!, where L
 *            = sum of exponents in the prime factorization of n, m_j are
 *            those exponents, and φ is Euler's totient. This counts the
 *            distinct necklaces of the multiset of prime indices of n (>1)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>


static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;

/* Build the list of all primes ≤ limit.  Overwrites any previous list,
   if the current list is insufficient. */
static void build_primes_upto(idx_t limit)
{
    if (prime_list && prime_cnt > 0 && prime_list[prime_cnt-1] >= limit)
        return;                   /* already enough */

    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return;                       /* failure: keep old list */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (limit >= 0) {
        is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;
    }

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;                       /* failure */
    }

    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_list[k++] = (uint64_t)i;

    free(is_prime);
    if (prime_list)
        free(prime_list);
    prime_list = new_list;
    prime_cnt  = cnt;
}

/* Compute Euler's totient of x (x ≤ 13 here).  Returns a 64‑bit value. */
static idx_t totient(idx_t x)
{
    idx_t result = x;
    idx_t t = x;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            result = result / p * (p - 1);
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1)
        result = result / t * (t - 1);
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                      /* convention for a(1) */

    /* Ensure we have primes up to n for factorisation */
    build_primes_upto(n);

    /* Factorise n and collect exponents */
    idx_t tmp = n;
    idx_t exps[64];
    size_t ecnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            exps[ecnt++] = cnt;   /* store exponent only */
        }
    }
    if (tmp > 1) {                  /* remaining prime factor */
        exps[ecnt++] = 1;
    }

    /* Total length L of the multiset */
    idx_t L = 0;
    for (size_t i = 0; i < ecnt; ++i) L += exps[i];

    /* Pre‑compute factorials up to L */
    seq_t fac[64];
    fac[0] = 1;
    for (idx_t i = 1; i <= L; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;

    /* Burnside’s lemma over all divisors d of L */
    seq_t total = 0;
    for (idx_t d = 1; d <= L; ++d) {
        if (L % d != 0)
            continue;
        idx_t h = L / d;                 /* cycle length */

        /* Check divisibility of all exponents by h */
        bool ok = true;
        for (size_t i = 0; i < ecnt; ++i) {
            if (exps[i] % h != 0) {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        /* φ(L/d) */
        idx_t phi_val = totient(h);      /* h = L/d */

        /* Denominator product of factorials of (exps[i]/h) */
        seq_t denom = 1;
        for (size_t i = 0; i < ecnt; ++i) {
            idx_t q = exps[i] / h;
            denom = denom * fac[q];
        }

        /* Contribution of this divisor */
        seq_t contrib = fac[d] * (seq_t)phi_val / denom;
        total = total + contrib;
    }

    /* Final division by L */
    seq_t result = total / (seq_t)L;
    return result;
}
