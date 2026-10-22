/*
 * A347690  Values of k such that (product of first k squarefree numbers) /
 *          (sum of first k squarefree numbers) is an integer.
 * https://oeis.org/A347690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: a(n) returns the n‑th k such that
   (∏₁^k sₖ)/(∑₁^k sₖ) is an integer, where sₖ are the squarefree numbers.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LIMIT 2000000                     /* enough for n up to ≈10 000 */
#define MAX_PRIMES 180000                 /* primes ≤ LIMIT are ≈148 933 */
#define MAX_MATCHES 200000                /* safe upper bound for matches stored */

static int primes[MAX_PRIMES];
static int prime_count = 0;
static int prime_index[LIMIT + 1];          /* prime_index[p] = index in primes, -1 otherwise */
static int8_t mu[LIMIT + 1];
static uint32_t spf[LIMIT + 1];

static uint32_t prod_exp[MAX_PRIMES];       /* factor counts of the running product */
static seq_t sum_val = 0;                   /* running sum of the first k squarefree numbers */
static uint64_t next_sqfree = 1;            /* next candidates to test for squarefree */
static idx_t cur_k = 0;                     /* number of squarefree numbers examined */

static idx_t matches[MAX_MATCHES];          /* stored matching k values */
static size_t match_count = 0;

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes / linear sieve to fill primes, spf, mu, prime_index */
static void init_sieve(void) {
    int i, j;
    memset(spf, 0, sizeof(spf));
    memset(mu, 0, sizeof(mu));
    memset(prime_index, -1, sizeof(prime_index));

    mu[1] = 1;
    spf[1] = 1;

    for (i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {                     /* i is prime */
            spf[i] = i;
            prime_index[i] = prime_count;
            primes[prime_count++] = i;
            mu[i] = -1;
        }
        for (j = 0; j < prime_count; ++j) {
            int p = primes[j];
            long long val = (long long)p * i;
            if (val > LIMIT) break;
            spf[val] = p;
            if (p == spf[i]) {
                mu[val] = 0;
                break;
            } else {
                mu[val] = -mu[i];
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Check whether sum_val divides the product represented by prod_exp */
static bool sum_divides_product(void) {
    uint64_t v = (uint64_t)sum_val;
    int i;

    for (i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= v; ++i) {
        if (v % primes[i] == 0) {
            unsigned int e = 0;
            while (v % primes[i] == 0) {
                v /= primes[i];
                ++e;
            }
            if (prod_exp[i] < e) return false;
        }
    }

    if (v > 1) {          /* remaining prime factor */
        if (v > (uint64_t)LIMIT) return false;         /* prime too large, product lacks it */
        int idx = prime_index[(int)v];
        if (idx < 0 || prod_exp[idx] == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main function implementing the sequence */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        memset(prod_exp, 0, sizeof(prod_exp));
        initialized = true;
    }

    while (match_count < n) {
        /* find next squarefree number */
        while (next_sqfree <= LIMIT && mu[next_sqfree] == 0) ++next_sqfree;
        if (next_sqfree > LIMIT) {
            /* LIMIT too small – not expected for n ≤ 10 000 */
            return (seq_t)0;
        }
        unsigned int sf = (unsigned int)next_sqfree;
        ++next_sqfree;

        /* update running sum */
        sum_val += (seq_t)sf;

        /* factorize sf and update product exponents */
        unsigned int tmp = sf;
        while (tmp > 1) {
            int p = spf[tmp];
            int idx = prime_index[p];
            ++prod_exp[idx];
            do { tmp /= p; } while (tmp % p == 0);
        }

        ++cur_k;
        if (sum_divides_product()) {
            if (match_count < MAX_MATCHES) {  /* safe guard */
                matches[match_count++] = cur_k;
            }
        }
    }

    return (seq_t)matches[n - 1];
}
