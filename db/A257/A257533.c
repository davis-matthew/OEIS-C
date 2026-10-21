/*
 * A257533  Sum of the proper divisors of the n-th semiprime.
 * https://oeis.org/A257533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of proper divisors of the n-th semiprime (A001358(n))
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static uint64_t *spf = NULL;            /* smallest prime factor for [0 .. LIMIT] */
static uint64_t *semiprimes = NULL;     /* list of semiprimes */
static size_t sem_count = 0;            /* number of semiprimes stored */
static const uint64_t LIMIT = 200000;   /* large enough for the first 10 000 semiprimes */

/* Build spf array and collect semiprimes up to LIMIT. */
static void init_semiprimes(void)
{
    /* Allocate the spf array.  Cast the result for C++. */
    spf = (uint64_t *)calloc((size_t)(LIMIT + 1), sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);

    /* Standard sieve of Eratosthenes to fill spf. */
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = i;
            if (i <= LIMIT / i) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Allocate an initial buffer for semiprimes.  Resize as needed. */
    size_t sem_cap = 20000;
    semiprimes = (uint64_t *)malloc(sem_cap * sizeof(uint64_t));
    if (!semiprimes) exit(EXIT_FAILURE);

    /* Scan numbers and collect semiprimes. */
    for (uint64_t x = 4; x <= LIMIT; ++x) {
        uint64_t n = x;
        uint32_t cnt = 0;
        while (n > 1) {
            uint64_t f = spf[n];
            ++cnt;
            n /= f;
            if (cnt > 2) break;          /* more than two prime factors */
        }
        if (cnt == 2) {                    /* x is a semiprime */
            if (sem_count == sem_cap) {
                sem_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(semiprimes, sem_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                semiprimes = tmp;
            }
            semiprimes[sem_count++] = x;
        }
    }

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) init_semiprimes();

    if (n == 0 || (size_t)(n - 1) >= sem_count)
        return (seq_t)0;                     /* out‑of‑range guard */

    uint64_t sp = semiprimes[(size_t)(n - 1)];
    uint64_t p  = spf[sp];
    uint64_t q  = sp / p;

    if (p == q)
        return (seq_t)p;                     /* square of a prime */
    else
        return (seq_t)(p + q);               /* sum of the two distinct primes */
}
