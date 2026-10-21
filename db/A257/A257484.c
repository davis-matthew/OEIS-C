/*
 * A257484  Numbers k such that lambda(sum of divisors of k that are congruent
 *          to 0 mod 3) = lambda(sum of divisors of k that are congruent to 1
 *          mod 3) = lambda(sum of divisors of k that are congruent to 2 mod
 *          3) where lambda is the Carmichael function (A002322).
 * https://oeis.org/A257484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 573, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257484: Numbers k such that
   λ( Σ_{d|k, d≡0 (3)} d ) = λ( Σ_{d|k, d≡1 (3)} d ) = λ( Σ_{d|k, d≡2 (3)} d ),
   where λ is the Carmichael function (OEIS A002322).
   The function a(idx_t n) returns the n‑th such number (n≥1).
   A simple sieve + divisor enumeration approach with caching. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"     /* gcd(seq_t, seq_t) */

/* ------------------------------------------------------------------ */
/* Prime sieve: generate all primes up to 2,000,000 (~1.3e5 primes) */
static uint32_t *primes = NULL;   /* prime list */
static size_t primes_cnt = 0;     /* number of primes */

static void init_primes(void)
{
    if (primes_cnt != 0) return;          /* already initialized */

    const uint32_t MAXP = 2000000;
    bool *is_prime = (bool*)calloc(MAXP + 1, sizeof(bool));    /* C++ cast */
    if (!is_prime) abort();
    for (uint32_t i = 2; i <= MAXP; ++i) is_prime[i] = true;

    for (uint32_t i = 2; i * i <= MAXP; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= MAXP; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (uint32_t i = 2; i <= MAXP; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));       /* C++ cast */
    if (!primes) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= MAXP; ++i)
        if (is_prime[i]) primes[idx++] = i;

    primes_cnt = cnt;
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Compute Carmichael λ for a 64‑bit integer n. */
static seq_t lambda_of_uint64(uint64_t n)
{
    if (n == 0) return (seq_t)0;      /* unused */
    if (n == 1) return (seq_t)1;

    seq_t l = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t lambda_p;
            if (p == 2) {
                if (e == 1)       lambda_p = (seq_t)1;
                else if (e == 2)  lambda_p = (seq_t)2;
                else              lambda_p = ((seq_t)1) << (e - 2);
            } else {
                seq_t pow_pe_minus1 = (seq_t)1;
                for (unsigned k = 1; k < e; ++k) pow_pe_minus1 *= p;
                lambda_p = pow_pe_minus1 * (p - 1);
            }
            seq_t g = gcd(l, lambda_p);
            l = (l / g) * lambda_p;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        uint64_t p = tmp;
        seq_t lambda_p = (p == 2) ? (seq_t)1 : (p - 1);
        seq_t g = gcd(l, lambda_p);
        l = (l / g) * lambda_p;
    }
    return l;
}

/* ------------------------------------------------------------------ */
/* Recursively generate all divisors of k from its prime factorisation
   and add them to sum[0], sum[1], or sum[2] according to divisor % 3. */
static void gen_divisors(int idx, uint64_t cur,
                         const uint64_t pf[], const unsigned exp[],
                         size_t fac_cnt, seq_t sum[3])
{
    if (idx == fac_cnt) {
        int r = cur % 3;
        sum[r] += (seq_t)cur;
        return;
    }
    uint64_t p = pf[idx];
    uint64_t val = 1;
    for (unsigned e = 0; e <= exp[idx]; ++e) {
        gen_divisors(idx + 1, cur * val, pf, exp, fac_cnt, sum);
        val *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Test whether k satisfies the condition for the sequence. */
static bool k_is_candidate(uint64_t k)
{
    uint64_t pf[32];
    unsigned exp[32];
    size_t fac = 0;

    uint64_t tmp = k;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[fac] = p;
            exp[fac] = e;
            ++fac;
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        pf[fac] = tmp;
        exp[fac] = 1;
        ++fac;
    }

    seq_t sum[3] = {0, 0, 0};
    gen_divisors(0, 1, pf, exp, fac, sum);

    if (sum[0] == 0 || sum[1] == 0 || sum[2] == 0) return false;

    seq_t l0 = lambda_of_uint64((uint64_t)sum[0]);
    seq_t l1 = lambda_of_uint64((uint64_t)sum[1]);
    seq_t l2 = lambda_of_uint64((uint64_t)sum[2]);

    return (l0 == l1) && (l1 == l2);
}

/* ------------------------------------------------------------------ */
/* Cache of computed sequence values */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;      /* number stored */
static size_t terms_cap = 0;      /* current allocation */
static uint64_t next_k = 2;       /* next integer to test */

/* Ensure the cache contains at least n terms (n≥1). */
static void ensure_terms(idx_t n)
{
    init_primes();

    while (terms_cnt < (size_t)n) {
        if (k_is_candidate(next_k)) {
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));  /* C++ cast */
                if (!tmp) abort();
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */
    ensure_terms(n);
    return terms[n - 1];
}
