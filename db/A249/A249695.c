/*
 * A249695  a(n)=0, if A249441(n)=0; otherwise, a(n) is the smallest i such
 *          that A249441(n)^2 divides binomial(n,i).
 * https://oeis.org/A249695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=0, if A249441(n)=0; otherwise a(n) is the smallest i such
 *            that A249441(n)^2 divides binomial(n,i)
 * Range:     u8 n <= 894, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249441(n) is the smallest prime p such that p^2 divides some binomial coefficient C(n,k). */
/* The function below computes a(n) for all n>=0. */

#include <stdbool.h>
#include <stddef.h>

/* Helper: compute primes up to MAX_N once. */
#define MAX_N 10000

static unsigned int primes[MAX_N + 1];
static size_t prime_count = 0;

/* Compute primes using simple sieve, called once. */
static void init_primes(void)
{
    if (prime_count != 0) return; /* already initialized */
    bool is_comp[MAX_N + 1] = {false};
    for (unsigned int i = 2; i <= MAX_N; ++i) {
        if (!is_comp[i]) {
            primes[prime_count++] = i;
            if ((unsigned long long)i * i <= MAX_N) {
                for (unsigned int j = i * i; j <= MAX_N; j += i)
                    is_comp[j] = true;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Return 0 for negative or trivial indices. */
    if (n <= 1) return (seq_t)0;

    init_primes();          /* Ensure primes are ready. */
    static unsigned char vp[MAX_N + 2];  /* exponent of current prime in numbers 0..n. */

    unsigned int limit = (unsigned int)n;
    for (size_t pi = 0; pi < prime_count && primes[pi] <= limit; ++pi) {
        unsigned int p = primes[pi];

        /* Precompute exponent of p in 1..n. */
        vp[0] = 0;
        for (idx_t m = 1; m <= limit; ++m) {
            unsigned char cnt = 0;
            idx_t t = m;
            while (t % p == 0) {
                ++cnt;
                t /= p;
            }
            vp[(size_t)m] = cnt;
        }

        /* Scan binomial coefficients. */
        unsigned long long exp = 0;               /* current exponent of p in C(n,k). */
        for (idx_t k = 0; k < limit; ++k) {       /* k runs 0..n-1, we will check C(n,k+1) */
            exp += vp[(size_t)(limit - k)] - vp[(size_t)(k + 1)];
            if (exp >= 2) {
                /* Found smallest i = k+1 where p^2 divides C(n,i). */
                return (seq_t)(k + 1);
            }
        }
        /* If no i found for this prime, try next prime. */
    }
    /* No prime found, return 0. */
    return (seq_t)0;
}
