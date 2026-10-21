/*
 * A256017  Least integer k > n such that all divisors of n are exactly the
 *          first divisors of k in increasing order.
 * https://oeis.org/A256017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p where p is the smallest prime satisfying if p | n :
 *            p^(exp+1) > n (exp is the exponent of p in n) else : p > n This
 *            ensures that every divisor of n is exactly the first divisors of
 *            k = n·p
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *prime_tbl = NULL;
static size_t prime_cnt   = 0;

// Simple sieve to generate all primes up to a fixed bound.
static void init_primes(void)
{
    if (prime_tbl != NULL) return;

    /* 20000 is far larger than the largest n (≤10000) and
       the next prime after 10000 is 10007. */
    const size_t LIM = 20000;
    bool *sieve = (bool *)malloc(LIM + 1);
    for (size_t i = 2; i <= LIM; ++i) sieve[i] = true;

    for (size_t i = 2; i * i <= LIM; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIM; j += i)
                sieve[j] = false;

    size_t cnt = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (sieve[i]) ++cnt;

    prime_tbl = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    prime_cnt = cnt;
    size_t pos = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (sieve[i]) prime_tbl[pos++] = (uint32_t)i;

    free(sieve);
}

// Main sequence function
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          // behaviour for n<1 is irrelevant

    init_primes();                         // make sure primes are ready

    /* Factorise n and store the primes and exponents. */
    uint32_t pf[10];
    uint32_t exp[10];
    size_t pcnt = 0;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= tmp; ++i) {
        uint32_t p = prime_tbl[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[pcnt]   = p;
            exp[pcnt]  = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {                     // remaining prime factor
        pf[pcnt]   = (uint32_t)tmp;
        exp[pcnt]  = 1;
        ++pcnt;
    }

    /* Find the smallest prime p satisfying the condition. */
    uint32_t best_p = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_tbl[i];

        /* Check if p divides n. */
        size_t j;
        for (j = 0; j < pcnt; ++j)
            if (pf[j] == p) break;

        bool ok = false;
        if (j < pcnt) {                               // p divides n
            unsigned long long pow_val = 1;
            uint32_t e = exp[j] + 1;
            while (e--) pow_val *= (unsigned long long)p;
            if (pow_val > (unsigned long long)n) ok = true;
        } else {                                      // p does not divide n
            if ((uint64_t)p > n) ok = true;
        }

        if (ok) { best_p = p; break; }
    }

    /* best_p is always found (next prime after n works). */
    return ((seq_t)n) * ((seq_t)best_p);
}
