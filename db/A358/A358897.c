/*
 * A358897  Numbers k such that p(k)^p(k) < p(k+1)^p(k-1), where p(k) =
 *          prime(k).
 * https://oeis.org/A358897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358897: Numbers k such that p(k)^p(k) < p(k+1)^p(k-1)  */
/* Computation uses the inequality p(k)*log(p(k)) < p(k-1)*log(p(k+1))  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static uint64_t *primes = NULL;          /* primes[i] = p(i+1) */
static long double *prime_logs = NULL;   /* log of primes */
static size_t prime_cnt = 0;

static seq_t *terms = NULL;              /* collected k values (1‑based) */
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

/* Ensure primes up to a fixed limit and precompute all qualifying k */
static void init(void) {
    static int done = 0;
    if (done) return;
    done = 1;

    /* Sieve limit sufficiently large for 10 000 terms (≈ 1.3 M primes) */
    const idx_t LIMIT = 20000000;
    unsigned char *isprime = (unsigned char *)calloc(LIMIT + 1, sizeof(unsigned char));
    if (!isprime) exit(1);
    isprime[2] = 1;
    for (idx_t i = 3; i <= LIMIT; i += 2) isprime[i] = 1;

    for (idx_t i = 3; i * i <= LIMIT; i += 2) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += 2 * i)
                isprime[j] = 0;
        }
    }

    /* Count primes */
    prime_cnt = 1;                  /* for prime 2 */
    for (idx_t i = 3; i <= LIMIT; i += 2)
        if (isprime[i]) ++prime_cnt;

    primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    prime_logs = (long double *)malloc(prime_cnt * sizeof(long double));
    if (!primes || !prime_logs) exit(1);

    /* Store primes and their logs */
    size_t idx = 0;
    primes[idx] = 2;
    prime_logs[idx] = logl((long double)2);
    ++idx;
    for (idx_t i = 3; i <= LIMIT; i += 2) {
        if (isprime[i]) {
            primes[idx] = (uint64_t)i;
            prime_logs[idx] = logl((long double)i);
            ++idx;
        }
    }

    free(isprime);

    /* Compute all indices k satisfying the inequality */
    for (size_t k = 2; k + 1 < prime_cnt; ++k) {   /* k>=2 because of k-1 */
        long double lhs = primes[k-1] * prime_logs[k-1];   /* p(k) * log p(k) */
        long double rhs = primes[k-2] * prime_logs[k];     /* p(k-1) * log p(k+1) */
        if (lhs < rhs) {
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) exit(1);
            }
            terms[terms_cnt++] = (seq_t)k;
        }
    }
}

/* External linkage as required */
seq_t a(idx_t n) {
    init();
    if (n == 0 || n > terms_cnt) return (seq_t)0;   /* undefined for out‑of‑range */
    return terms[n - 1];                            /* a(1) is first stored k */
}
