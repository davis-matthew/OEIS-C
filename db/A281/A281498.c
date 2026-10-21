/*
 * A281498  Least k >= 0 such that 2^k * n! is not a refactorable number.
 * https://oeis.org/A281498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 0 such that 2^k * n! is not a refactorable
 *            number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition by computing the prime‑exponent
   factorisations of n! and of τ(2^k·n!) and testing divisibility.
   A sieve is used once to generate all primes up to 20000, which is
   sufficient for factoring the numbers e_p+1 (e_p <= 10000). */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRIME   20000          /* maximal prime used for factoring */
#define MAX_PRIMES  2262           /* number of primes <= 20000 */

static int  primes[MAX_PRIMES];      /* list of primes */
static int  prime_count;             /* number of primes */
static int  prime_index_of[MAX_PRIME + 1]; /* mapping value -> index in primes */
static bool primes_initialized = false;

/* One‑time sieve to fill primes[] and prime_index_of[] */
static void init_primes(void)
{
    if (primes_initialized) return;
    bool comp[MAX_PRIME + 1];
    memset(comp, 0, sizeof(comp));
    memset(prime_index_of, -1, sizeof(prime_index_of));
    prime_count = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!comp[i]) {
            primes[prime_count] = i;
            prime_index_of[i] = prime_count;
            ++prime_count;
            if ((long long)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i) comp[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* Factor the integer 'term' (>=2) and add its prime exponents to tauExp[].
   The array tauExp[] is indexed by the global primes list. */
static void factor_term(int term, int *tauExp)
{
    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if (p * p > term) break;
        if (term % p == 0) {
            int cnt = 0;
            while (term % p == 0) {
                term /= p;
                ++cnt;
            }
            tauExp[i] += cnt;
        }
    }
    if (term > 1) {
        int idx = prime_index_of[term];
        if (idx >= 0) tauExp[idx] += 1;
    }
}

/* Main function: returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    init_primes();

    /* exponent of each prime in n! (e[prime_index]) */
    static int e[MAX_PRIMES];
    for (int i = 0; i < prime_count; ++i) e[i] = 0;

    /* compute exponents for primes <= n */
    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if (p > n) break;
        int exp = 0;
        int t = (int)n;
        while (t) {
            t /= p;
            exp += t;
        }
        e[i] = exp;
    }

    /* iterate over k until the condition fails */
    idx_t k = 0;
    for (;;) {
        static int tauExp[MAX_PRIMES];
        for (int i = 0; i < prime_count; ++i) tauExp[i] = 0;

        /* consider all primes that can appear in n! or in 2^k factor */
        for (int i = 0; i < prime_count; ++i) {
            int p = primes[i];
            /* we need to treat p = 2 even if n < 2 */
            if (p > n && p > 2) break;
            int term = (p == 2 ? e[i] + (int)k : e[i]) + 1;
            if (term <= 1) continue;
            factor_term(term, tauExp);
        }

        /* check divisibility: for every prime, exponent in m >= exponent in τ(m) */
        bool ok = true;
        for (int i = 0; i < prime_count; ++i) {
            if (tauExp[i] > 0) {
                int exp_in_m = e[i] + (primes[i] == 2 ? (int)k : 0);
                if (exp_in_m < tauExp[i]) {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) return (seq_t)k;
        ++k;
    }
}
