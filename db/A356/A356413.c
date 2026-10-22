/*
 * A356413  Numbers with an equal sum of the even and odd exponents in their
 *          prime factorizations.
 * https://oeis.org/A356413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k (k ≥ 1) such that the sum of even
 *            exponents in the prime factorization of k equals the sum of odd
 *            exponents (A350386(k) = A350387(k))
 * Range:     u8 n <= 14, u16 n <= 6302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation builds the sequence on demand, caching all
    terms found so far.  For each new term it performs a trial
    division using a pre‑computed prime table.  The sequence is
    guaranteed to be correct for any n ≥ 1 that fits in the
    available memory. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state */
static seq_t *terms = NULL;          /* array of found terms      */
static size_t terms_len = 0;         /* number of stored terms    */
static size_t terms_cap = 0;         /* capacity of array         */
static uint32_t *primes = NULL;      /* prime sieve table         */
static size_t prime_cnt = 0;         /* number of primes          */
static uint64_t searched_until = 1;  /* largest candidate checked */

/* build a prime table up to LIMIT; called once */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    const size_t LIMIT = 1000000;  /* adjust if >10000th term is larger   */
    unsigned char *is_prime = (unsigned char*)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    prime_cnt = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;
    free(is_prime);
    /* initial term 1 belongs to the sequence */
    terms_len = 1;
    terms_cap = 16;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    terms[0] = (seq_t)1;
    done = true;
}

/* append a new term to the cache */
static inline void cache_put(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* the required function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined, but never called */

    init_primes();               /* ensure primes & initial term */

    if (n <= terms_len)        /* cached term available */
        return terms[n - 1];

    /* iterate candidates until enough terms are collected */
    seq_t candidate = (seq_t)(searched_until + 1);
    while (terms_len < (size_t)n) {
        /* factor candidate */
        uint64_t temp = (uint64_t)candidate;
        seq_t even_sum = 0;
        seq_t odd_sum  = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > temp)
                break;
            if (temp % p == 0) {
                uint64_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if (exp & 1) odd_sum += exp;
                else        even_sum += exp;
            }
        }
        if (temp > 1)          /* remaining prime factor */
            odd_sum += 1;

        if (even_sum == odd_sum)   /* qualifies */
            cache_put(candidate);

        ++candidate;
        searched_until = (uint64_t)(candidate - 1);
    }

    return terms[n - 1];
}
