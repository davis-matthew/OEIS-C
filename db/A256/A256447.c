/*
 * A256447  Number of integers in range (prime(n)^2)+1 ..
 *          (prime(n)*prime(n+1)) whose smallest prime factor is at least
 *          prime(n): a(n) = A250477(n) - A250474(n).
 * https://oeis.org/A256447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers in [p_n^2+1, p_n·p_{n+1}] whose
 *            smallest prime factor is ≥ p_n Computation: segmented sieve over
 *            that interval marking numbers divisible by any prime < p_n
 * Range:     u8 n <= 60, u16 n <= 564, u32 n <= 564, u64 n <= 564,
 *            u128 n <= 564, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/* Prime sieve: pre‑compute all primes up to a fixed limit.
   That limit is large enough to contain the 10001st prime (≈104743). */
#define PRIME_LIMIT 2000000               /* primes up to this value      */
#define PRIME_ARRAY_SIZE 200000           /* enough for ~200k primes      */

static uint32_t primes[PRIME_ARRAY_SIZE] = {0};
static size_t   prime_cnt    = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    /* classic Eratosthenes sieve with a byte array */
    uint8_t *sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    if (!sieve) return;  /* allocation failure – caller will handle 0 */

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt < PRIME_ARRAY_SIZE)
                primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
    primes_ready = true;
}

/* ------------------------------------------------------------------- */
/* Main sequence function.  External linkage is required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n=0 – return 0 as sentinel */

    init_primes();

    /* Ensure we have at least n+1 primes. */
    if (n + 1 >= prime_cnt)
        return 0;                /* exceeded precomputed limit – failure case */

    uint32_t p_n   = primes[n - 1];  /* primes are 0‑based: primes[0] = 2   */
    uint32_t p_np1 = primes[n];

    /* Interval bounds.  All values fit into 64‑bit unsigned. */
    uint64_t L = (uint64_t)p_n * p_n + 1;
    uint64_t R = (uint64_t)p_n * p_np1;
    uint64_t N = R - L + 1;          /* number of integers in the interval */

    /* Allocate a byte array for marking.  Each byte stays 0/1. */
    uint8_t *marked = (uint8_t *)calloc(N, 1);
    if (!marked) return 0;          /* allocation failure */

    /* Mark all numbers in [L,R] that are divisible by any prime < p_n. */
    for (size_t i = 0; i < prime_cnt && primes[i] < p_n; ++i) {
        uint32_t q = primes[i];
        uint64_t start = (L + q - 1) / q * q;     /* first multiple of q ≥ L */
        for (uint64_t m = start; m <= R; m += q)
            marked[m - L] = 1;
    }

    /* Count unmarked numbers: these have smallest prime factor ≥ p_n. */
    seq_t count = 0;
    for (uint64_t i = 0; i < N; ++i)
        if (!marked[i])
            ++count;

    free(marked);
    return count;
}
