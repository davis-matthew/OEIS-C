/*
 * A282038  (Sum of the quadratic nonresidues of prime(n)) - (sum of the
 *          quadratic residues of prime(n)).
 * https://oeis.org/A282038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sum of quadratic nonresidues of prime(n)) * - (sum of
 *            quadratic residues of prime(n))
 * Range:     i8 n <= 14, i16 n <= 172, i32 n <= 5677, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* maximum prime to pre‐compute (covers n up to ~148933 primes) */
#define MAX_PRIME_LIMIT 2000000

/* global sieve and prime list */
static unsigned char *prime_flags = NULL;   /* 0/1 flags for sieve */
static uint64_t *prime_list  = NULL;        /* array of primes */
static size_t prime_count = 0;             /* number of primes stored */

/* build the list of primes once */
static void init_primes(void)
{
    if (prime_flags != NULL)                /* already initialised */
        return;

    /* sieve of Eratosthenes */
    prime_flags = (unsigned char*)malloc(MAX_PRIME_LIMIT + 1);
    if (!prime_flags)
        return;                            /* allocation failed */

    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        prime_flags[i] = 1;
    prime_flags[0] = prime_flags[1] = 0;

    for (unsigned long long i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (prime_flags[i])
            for (unsigned long long j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                prime_flags[j] = 0;

    /* count primes */
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        if (prime_flags[i])
            ++prime_count;

    /* allocate prime array */
    prime_list = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) {
        free(prime_flags);
        prime_flags = NULL;
        return;
    }

    size_t idx = 0;
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        if (prime_flags[i])
            prime_list[idx++] = i;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)           /* offset is 1 – undefined for n<1 */
        return 0;

    init_primes();
    if (n > (idx_t)prime_count)   /* n beyond pre‑computed primes */
        return 0;

    uint64_t p = prime_list[n - 1];  /* the n‑th prime */

    /* special case p = 2 */
    if (p == 2)
        return -1;               /* a(1) = -1 */

    /* S_total = sum_{k=1}^{p-1} k = p*(p-1)/2 */
    seq_t p_seq   = (seq_t)p;
    seq_t S_total = p_seq * (p_seq - 1) / 2;

    /* sum of distinct quadratic residues modulo p (p odd) */
    seq_t sum_res = 0;
    idx_t half = (idx_t)((p - 1) / 2);
    for (idx_t i = 1; i <= half; ++i) {
        uint64_t sq = (i * i) % p;
        sum_res += (seq_t)sq;
    }

    /* a(n) = S_total - 2 * sum_res */
    return S_total - (seq_t)2 * sum_res;
}
