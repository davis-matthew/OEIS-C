/*
 * A276000  Least k such that n! divides phi(k!) (k > 0).
 * https://oeis.org/A276000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n! divides φ(k!)
 * Range:     u8 n <= 130, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pre‑computed small‑prime sieve and auxiliary tables */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum value to which we pre-allocate tables.
   This is comfortably larger than 2·n for all expected n (e.g. n≤10000). */
#define MAX_K 500000
/* Upper bound for number of primes ≤ MAX_K (≈41538) */
#define MAX_PRIMES 50000

static int spf[MAX_K + 1];                /* smallest prime factor */
static int primes[MAX_PRIMES];
static int prime_index[MAX_K + 1];        /* -1 for composite, else index in primes[] */
static int primes_count = 0;
static bool sieve_done = false;

/* Helper to build the sieve once */
static void build_sieve(void)
{
    if (sieve_done) return;
    for (int i = 0; i <= MAX_K; ++i) spf[i] = i;
    for (int i = 2; i * i <= MAX_K; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_K; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    for (int i = 0; i <= MAX_K; ++i) prime_index[i] = -1;
    for (int i = 2; i <= MAX_K; ++i) {
        if (spf[i] == i) {
            prime_index[i] = primes_count;
            primes[primes_count++] = i;
        }
    }
    sieve_done = true;
}

/* Exponent of prime p in n! */
static inline int factorial_exponent(unsigned int n, int p)
{
    int exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* Main function implementing A276000 */
seq_t a(idx_t n)
{
    if (!sieve_done) build_sieve();

    /* Special cases given directly in the OEIS list */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)3;

    unsigned int ui = (unsigned int)n;
    /* Determine primes needed for this n */
    int primes_needed = 0;
    static int need[MAX_PRIMES];
    for (int i = 0; i < primes_count && primes[i] <= ui; ++i) {
        need[i] = factorial_exponent(ui, primes[i]);  /* always >0 for primes ≤ n */
        ++primes_needed;
    }

    /* Arrays for current search state */
    static int e_fact[MAX_PRIMES];
    static int e_phi[MAX_PRIMES];
    for (int i = 0; i < primes_needed; ++i) {
        e_fact[i] = 0;
        e_phi[i] = 0;
    }

    int deficits = primes_needed;   /* how many primes still need more exponents */
    unsigned int k = 1;

    while (true) {
        k++;                      /* next candidate k */

        /* Factorize k */
        unsigned int tmp = k;
        while (tmp > 1) {
            int sp = spf[tmp];
            int cnt = 0;
            while (tmp % sp == 0) {
                tmp /= sp;
                ++cnt;
            }
            int idx = prime_index[sp];
            if (idx >= 0 && idx < primes_needed) {
                int prev_use = e_fact[idx];
                e_fact[idx] += cnt;
                int delta = (prev_use == 0) ? (cnt - 1) : cnt;
                if (delta > 0) {
                    int prev_phi = e_phi[idx];
                    e_phi[idx] += delta;
                    if (prev_phi < need[idx] && e_phi[idx] >= need[idx])
                        --deficits;
                }
            }
        }

        /* If k itself is prime, add factorization of k-1 once */
        if (k >= 2 && spf[k] == k) {
            unsigned int tmp2 = k - 1;
            while (tmp2 > 1) {
                int sp = spf[tmp2];
                int cnt = 0;
                while (tmp2 % sp == 0) {
                    tmp2 /= sp;
                    ++cnt;
                }
                int idx = prime_index[sp];
                if (idx >= 0 && idx < primes_needed) {
                    int prev_phi = e_phi[idx];
                    e_phi[idx] += cnt;
                    if (prev_phi < need[idx] && e_phi[idx] >= need[idx])
                        --deficits;
                }
            }
        }

        if (deficits == 0) break;
    }

    return (seq_t)k;
}
