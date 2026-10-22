/*
 * A331019  a(n) is the least k such that the
 *          denominator(sigma(sigma(k*n))/(k*n)) equals n.
 * https://oeis.org/A331019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k such that sigma(sigma(k*n))/(k*n) has
 *            denominator n
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A331019 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static size_t primes_cnt = 0;
static bool primes_initialized = false;
#define PRIME_MAX 1000000          /* sufficient for all practical inputs */
#define PRIMES_CAP 80000           /* number of primes up to PRIME_MAX is 78498 */

/* allocate a fixed array for the primes */
static uint32_t primes[PRIMES_CAP];

/* Simple sieve of Eratosthenes to fill primes array */
static void init_primes(void)
{
    if (primes_initialized) return;

    size_t limit = PRIME_MAX;
    bool *composite = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!composite) exit(1);          /* unlikely to fail on these limits */

    memset(composite, 0, (limit + 1) * sizeof(bool));

    for (size_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            if (primes_cnt < PRIMES_CAP)
                primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    composite[j] = true;
            }
        }
    }

    free(composite);
    primes_initialized = true;
}

/* Greatest common divisor for seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* σ(m) for 64‑bit integer m, result in seq_t */
static seq_t sigma_of_idx(idx_t n)
{
    if (n <= 1) return (seq_t)n;

    idx_t tmp = n;
    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        if (tmp % p == 0) {
            seq_t sum = 1;
            seq_t pow = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pow *= (seq_t)p;
                sum += pow;
            }
            result *= sum;
        }
    }

    if (tmp > 1) {           /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)tmp);
    }

    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* no such term, harmless guard */
    init_primes();

    idx_t k = 1;
    while (1) {
        /* m = k*n  (product fits in idx_t for the intended range) */
        idx_t m = k * n;

        seq_t sigma_m = sigma_of_idx(m);

        /* sigma_m is expected to fit into idx_t for the next call */
        idx_t sigma_m_int = (idx_t)sigma_m;

        seq_t sigma_sigma = sigma_of_idx(sigma_m_int);

        seq_t g = gcd_seq_t(sigma_sigma, (seq_t)m);

        if (g == (seq_t)k) return k;   /* condition satisfied */

        ++k;
    }
}
