/*
 * A336297  Prime numbers p such that equation x = p*sopf(x) (where sopf(x) is
 *          the sum of distinct prime factors of x) has exactly 1 solution in
 *          positive integers.
 * https://oeis.org/A336297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes OEIS A336297 – prime numbers p such that the equation x
 *            = p * sopf(x) has exactly one positive integer solution. This
 *            implementation finds all such primes up to a fixed large limit
 *            and returns the n-th term (offset 1) from the precomputed list
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Parameters of the algorithm.  The chosen limit is large enough to
   produce at least 10 000 terms of the sequence. */
#define LIMIT 2000000U

/* Static working data.  These objects are allocated only once. */
static bool initialized = false;
static seq_t *seq_values = NULL;
static size_t seq_len = 0;

static void init_sequence(void)
{
    /* Allocate arrays. */
    uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    unsigned char *isPrime = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
    unsigned char *excluded = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
    int *sumD = (int *)malloc((LIMIT + 1) * sizeof(int));
    seq_values = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));

    if (!spf || !isPrime || !excluded || !sumD || !seq_values) {
        /* Memory failure – abort program. */
        exit(EXIT_FAILURE);
    }

    /* ---- 1. Sieve for smallest prime factor (spf) and primality flag. */
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        spf[i] = 0;
        isPrime[i] = 0;
        excluded[i] = 0;
    }
    spf[0] = 0; spf[1] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            isPrime[i] = 1;
            if ((uint64_t)i * i <= (uint64_t)LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0)
                        spf[j] = i;
                }
            }
        }
    }

    /* ---- 2. Sum of distinct prime factors (sumD). */
    sumD[0] = sumD[1] = 0;
    for (uint32_t n = 2; n <= LIMIT; ++n) {
        uint32_t p = spf[n];
        uint32_t m = n / p;
        if (m % p == 0)
            sumD[n] = sumD[m];               /* p already counted */
        else
            sumD[n] = sumD[m] + (int)p;      /* new prime factor */
    }

    /* ---- 3. Mark primes that have an extra solution. */
    for (uint32_t n = 4; n <= LIMIT; ++n) {
        if (spf[n] == n)          /* n is prime – skip */
            continue;
        int diff = (int)n - sumD[n];
        if (diff >= 2 && (uint32_t)diff <= LIMIT &&
            spf[diff] == diff &&   /* diff is prime    */
            n % diff != 0) {       /* p does not divide n   */
            excluded[diff] = 1;
        }
    }

    /* ---- 4. Build the sequence. */
    seq_len = 0;
    for (uint32_t n = 2; n <= LIMIT; ++n) {
        if (isPrime[n] && !excluded[n]) {
            seq_values[seq_len++] = (seq_t)n;
        }
    }

    /* Free temporary arrays (they are no longer needed). */
    free(spf);
    free(isPrime);
    free(excluded);
    free(sumD);

    initialized = true;
}

/* Public sequence function as required. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    /* n is 1‑based; ensure it's within bounds. */
    if (n == 0 || (idx_t)seq_len < n)
        return 0;                    /* out of range – safe default */

    return seq_values[(size_t)n - 1];
}
