/*
 * A348981  a(n) = Sum_{d|n} phi(n/d) * A322582(d), where A322582(n) = n -
 *          A003958(n), and A003958 is fully multiplicative with a(p) = (p-1).
 * https://oeis.org/A348981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * (d - prod_{p|d} (p-1)^{v_p(d)}),
 *            where phi is Euler's totient and the product is A003958(d)
 * Range:     u8 n <= 71, u16 n <= 4319, u32 n <= 9788, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi.h"        /* provides seq_t phi(idx_t m) */

static uint64_t *prime_list = NULL;   /* primes, 0‑indexed             */
static size_t   prime_count = 0;      /* number of primes              */
static idx_t    prime_limit = 0;      /* maximum prime computed        */

/* ensure that all primes up to `limit` are generated */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit)
        return;                      /* already computed             */

    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime)
        return;                      /* OOM → abort                     */

    for (size_t i = 2; i < sz; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i < sz; ++i) {
        if (!is_prime[i])
            continue;
        /* store prime */
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            (prime_count + 1) * sizeof(uint64_t));
        if (!tmp) {
            free(is_prime);
            return;                  /* OOM → abort                     */
        }
        prime_list = tmp;
        prime_list[prime_count++] = (uint64_t)i;

        if ((uint64_t)i * (uint64_t)i <= limit) {
            for (uint64_t j = (uint64_t)i * (uint64_t)i;
                 j <= (uint64_t)limit; j += (uint64_t)i)
                is_prime[(size_t)j] = false;
        }
    }

    free(is_prime);
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  a(n) : Dirichlet convolution of phi with A322582               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n==0 */

    /* primes needed for factorisation of n */
    ensure_primes_upto(n);

    /* factorise n: store primes and exponents */
    uint64_t  fac[16];
    int       exp[16];
    size_t    pcnt = 0;

    idx_t temp = n;
    for (size_t i = 0; i < prime_count &&
          (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p != 0)
            continue;
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        fac[pcnt]   = p;
        exp[pcnt++] = e;
    }
    if (temp > 1) {          /* remaining prime factor */
        fac[pcnt]   = (uint64_t)temp;
        exp[pcnt++] = 1;
    }

    /* generate all divisors of n together with their A003958 value */
    seq_t divs[256];
    seq_t prod[256];                 /* product of (p-1)^exp in the divisor */
    size_t dcnt = 1;
    divs[0]  = (seq_t)1;
    prod[0]  = (seq_t)1;

    for (size_t f = 0; f < pcnt; ++f) {
        uint64_t p = fac[f];
        int e     = exp[f];
        size_t old = dcnt;
        seq_t p_pow = (seq_t)1;
        seq_t pow_p_minus1 = (seq_t)1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= (seq_t)p;
            pow_p_minus1 *= (seq_t)(p - 1);
            for (size_t i = 0; i < old; ++i) {
                divs[dcnt] = divs[i] * p_pow;
                prod[dcnt] = prod[i] * pow_p_minus1;
                ++dcnt;
            }
        }
    }

    /* accumulate the convolution */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t  d   = (idx_t)divs[i];
        seq_t  a322 = divs[i] - prod[i];          /* A322582(d) */
        seq_t  phi_val = phi(n / d);              /* phi from utils */
        sum += phi_val * a322;
    }

    return sum;
}
